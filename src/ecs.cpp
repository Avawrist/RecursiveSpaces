// ================================================
// Title: ecs.cpp
// Description: The source file component structs
// ================================================

#include "ecs.hpp"

////////////////////////////////
// Struct Component Transform //
////////////////////////////////

Transform::Transform()
{
    position = Vec3F(0.0f, 0.0f, 0.0f);
    x_scale = 1.0f;
    y_scale = 1.0f;
    z_scale = 1.0f;
}

Transform::Transform(Vec3F _position)
{
    position = _position;
    x_scale = 1.0f;
    y_scale = 1.0f;
    z_scale = 1.0f;
}

Mat4F transformGetModel(const Transform& transform)
{
    // Column Major
    Mat4F model = Mat4F(transform.x_scale, 0.0f, 0.0f, transform.position.x,
	                0.0f, transform.y_scale, 0.0f, transform.position.y,
	                0.0f, 0.0f, transform.z_scale, transform.position.z,
	                0.0f, 0.0f, 0.0f, 1.0f);
    return model;
}

/////////////////////////////
// Struct Component Camera //
/////////////////////////////

Camera::Camera()
{
    is_selected = true;
    target      = Vec3F(0.0f, 0.0f, 0.0f);
}

///////////////////////////////
// Struct Component DirLight //
///////////////////////////////

DirLight::DirLight()
{
    color  = Vec3F(1.0f, 1.0f, 1.0f);
    dir    = Vec3F(1.0f, -1.0f, 1.0f);
    target = Vec3F(0.0f, 0.0f, 0.0f);
    offset = Vec3F(0.0f, 0.0f, 0.0f);
    speed  = 0.2f;
    ambient_strength = 0.4f;
}

/////////////////////////////////
// Struct Component PointLight //
/////////////////////////////////

PointLight::PointLight()
{
    color = Vec3F(1.0f, 1.0f, 1.0f);
    ambient_strength = 0.5f;
}

///////////////////////////////////
// Struct Component GridPosition //
///////////////////////////////////

GridPosition::GridPosition()
{
    position = Vec3F(0.0f, 0.0f, 0.0f);
}

GridPosition::GridPosition(Vec3F _position)
{
    position = _position; 
}

////////////////////////////
// Struct Component State //
////////////////////////////

State::State()
{
    inactive = true;
    input_cooldown = 0;
}

///////////////
// Struct AI //
///////////////

Node::Node()
{
    grid_pos = Vec3F(0.0f, 0.0f, 0.0f);
    g_cost = RG_MAX_WIDTH * RG_MAX_LENGTH * 3;
    h_cost = RG_MAX_WIDTH * RG_MAX_LENGTH * 3;
    f_cost = g_cost + h_cost;
    parent_p = NULL;
}

Node::Node(Vec3F start_pos, Vec3F _grid_pos, Vec3F target_pos, Node* _parent_p)
{
    grid_pos = _grid_pos;
    Vec2F dist_from_start  = Vec2F((float)abs((int)start_pos.x - (int)grid_pos.x),
	                           (float)abs((int)start_pos.z - (int)grid_pos.z));
    Vec2F dist_from_target = Vec2F((float)abs((int)target_pos.x - (int)grid_pos.x),
	                           (float)abs((int)target_pos.z - (int)grid_pos.z));
    g_cost = (uint)(dist_from_start.x + dist_from_start.y);
    h_cost = (uint)(dist_from_target.x + dist_from_target.y);
    f_cost = g_cost + h_cost;
    parent_p = _parent_p;
}

AI::AI()
{
    face_dir  = Vec3F(0.0f, 0.0f, 1.0f);
    next_move = MOVE_WALK;
}

////////////////////////////
// Struct EntityTemplates //
////////////////////////////

EntityTemplates::EntityTemplates()
{
    // Initialize all template components to 0
    memset(table, 0, TOTAL_ENTITY_TYPES * TOTAL_COMPONENT_TYPES * sizeof(uint));
}

//////////////////////
// Struct LevelGrid //
//////////////////////

RoomGrid::RoomGrid()
{
    memset(grid, -1, RG_MAX_WIDTH * RG_MAX_HEIGHT * RG_MAX_LENGTH * sizeof(int));
}

//////////////////
// Struct Level //
//////////////////

//////////////////////////////
// ActiveEntities Functions //
//////////////////////////////

ActiveEntities::ActiveEntities()
{
    // Initialize all types to 0 (meaning no type, vacancy)
    memset(types, NONE, MAX_ENTITIES * sizeof(uint));

    // Init all roomgrid IDs to -1
    for(uint i = 0; i < MAX_ENTITIES; i++)
    {
	roomgrid_ids[i] = -1;
    }
    
    count = 0;
}

int activeEntitiesCreateEntity(ActiveEntities& entities,
			       RoomGridLookup& roomgrid_lookup,
			       int room_grid_owner_id,
			       int room_grid_id,
			       Vec3F origin,
			       uint entity_type)
{
    // Returns entity ID on success, -1 on failure
    
    _assert(entity_type >= 0 && entity_type < TOTAL_ENTITY_TYPES);

    if (entities.count < MAX_ENTITIES)
    {
	// Activate entity
	entities.states[entities.count].inactive = false; 
	// Set new type
	entities.types[entities.count] = entity_type;
	// sets transform even if never used
	entities.transforms[entities.count] = Transform(origin);
	// if entity has a roomgrid component, store its lookup id and allocate the roomgrid
	if(entities.entity_templates.table[entity_type][COMPONENT_ROOM_GRID])
	{
	    entities.roomgrid_ids[entities.count] = room_grid_id;
	    if(room_grid_id > -1)
	    {
		roomgrid_lookup.roomgrid_pointers[room_grid_id] = new RoomGrid();
	    }
	}
	// if entity has a grid_position component, add to grid and set grid position
	if(entities.entity_templates.table[entity_type][COMPONENT_GRID_POSITION])
	{
	    entities.grid_positions[entities.count].roomgrid_owner_id = room_grid_owner_id;
	    entities.grid_positions[entities.count].position = origin;
	    if(room_grid_owner_id > -1)
	    {
		roomGridSetEntity(*roomgrid_lookup.roomgrid_pointers[room_grid_owner_id],
				  origin,
				  entities.count);
	    }
	}
	// Increase entity count
	entities.count++;
	// Return new entity ID
	return entities.count - 1;
    }
    OutputDebugStringA("ERROR - Failed to create entity - Max entities reached.\n");
    return -1;
}

void activeEntitiesMarkInactive(ActiveEntities& entities, uint entity_ID)
{
    _assert(entity_ID >= 0 && entity_ID < entities.count);
    
    // Sets state to inactive, system knows the entity is free to overwrite
    entities.states[entity_ID].inactive = true;
}

void activeEntitiesRemoveInactives(ActiveEntities& entities, RoomGridLookup& roomgrid_lookup)
{
    // Should be run after all other entity updates. Searches for inactive entities,
    // if found, overwrites the inactive with the active entity on the end of the arrays,
    // then decreases active_count by 1.
    // Repeats until the type array is completely searched.

    for(int i = entities.count - 1; i >= 0; i--)
    {
	if(entities.states[i].inactive)
	{	    
	    // If inactive entity is on the grid, set ID of removed entity
	    // back to -1 in the level grid:
	    if(entities.entity_templates.table[entities.types[i]][COMPONENT_GRID_POSITION])
	    {
		int roomgrid_id = entities.grid_positions[i].roomgrid_owner_id;
		if(roomgrid_id > -1)
		{
		    RoomGrid* grid_p = roomgrid_lookup.roomgrid_pointers[roomgrid_id];
		    Vec3F inactive_grid_position = entities.grid_positions[i].position;
		    roomGridRemoveEntity(*grid_p, inactive_grid_position);
		}
	    }
	    // If replacement entity is on the grid, update ID of last active
	    // entity in the grid, since it has changed
	    if(entities.entity_templates.table[entities.types[entities.count - 1]][COMPONENT_GRID_POSITION])
	    {
		int roomgrid_id = entities.grid_positions[entities.count - 1].roomgrid_owner_id;
		if(roomgrid_id > -1)
		{
		    RoomGrid* grid_p = roomgrid_lookup.roomgrid_pointers[roomgrid_id];
		    Vec3F active_grid_position = entities.grid_positions[entities.count - 1].position;
		    roomGridSetEntity(*grid_p, active_grid_position, i);
		}
	    }
	    // If the inactive entity had a room grid component, update the lookup table
	    // so it no longer points to the invalid room grid data
	    if(entities.entity_templates.table[entities.types[i]][COMPONENT_ROOM_GRID])
	    {
		int roomgrid_id = entities.roomgrid_ids[i];
		if(roomgrid_id > -1)
		{
		    roomgrid_lookup.roomgrid_pointers[roomgrid_id] = NULL;
		}

		// TODO: Removing a blockroom entity requires removing all of the entities
		//       it contains as well. But, those entities can't all be removed on this pass.
		//       Rewrite ECS code so inactive entities aren't updated?
	    }
	    
	    // Safe to copy grid position component now 
	    entities.grid_positions[i] = entities.grid_positions[entities.count - 1];
	    // Copy remaining component data from last active entity and fill inactive slot 
	    entities.transforms[i]     = entities.transforms[entities.count - 1];
	    entities.cameras[i]        = entities.cameras[entities.count - 1];
	    entities.dir_lights[i]     = entities.dir_lights[entities.count - 1];
	    entities.point_lights[i]   = entities.point_lights[entities.count - 1];
	    entities.states[i]         = entities.states[entities.count - 1];
	    entities.ai[i]             = entities.ai[entities.count - 1];
	    entities.roomgrid_ids[i]   = entities.roomgrid_ids[entities.count - 1];
            // Copy replacement entity's type last
	    entities.types[i] = entities.types[entities.count - 1];
	    // Set last active entity type to NONE for memory readability
	    entities.types[entities.count - 1] = NONE;
	    // Decrease entity count by one, effectively removing the inactive entity
	    // while preserving the entity on the end of the arrays. 
	    entities.count--;
	}
    }
}

////////////////////////
// RoomGrid Functions //
////////////////////////

int roomGridGetEntity(RoomGrid& room_grid, Vec3F pos)
{
    // Returns entity on success. Returns -1 if no entity. Returns -2 if out of bounds.
    
    // Check that target position is valid (within the grid)
    if(pos.x < 0.0f || pos.x >= RG_MAX_WIDTH ||
       pos.y < 0.0f || pos.y >= RG_MAX_HEIGHT ||
       pos.z < 0.0f || pos.z >= RG_MAX_LENGTH)
    {
	return INVALID_RANGE;
    }

    return room_grid.grid[(int)pos.x][(int)pos.y][(int)pos.z];
}

void roomGridSetEntity(RoomGrid& room_grid, Vec3F pos, int entity_ID)
{
    _assert(pos.x >= 0.0f && pos.x < RG_MAX_WIDTH);
    _assert(pos.y >= 0.0f && pos.y < RG_MAX_HEIGHT);
    _assert(pos.z >= 0.0f && pos.z < RG_MAX_LENGTH);
    
    room_grid.grid[(int)pos.x][(int)pos.y][(int)pos.z] = entity_ID;
}

void roomGridRemoveEntity(RoomGrid& room_grid, Vec3F pos)
{
    _assert(pos.x >= 0.0f && pos.x < RG_MAX_WIDTH);
    _assert(pos.y >= 0.0f && pos.y < RG_MAX_HEIGHT);
    _assert(pos.z >= 0.0f && pos.z < RG_MAX_LENGTH);
    
    room_grid.grid[(int)pos.x][(int)pos.y][(int)pos.z] = -1;
}

Vec3F roomGridFindNearestType(RoomGrid& room_grid, ActiveEntities& entities,
			       Vec3F cur_pos, uint target_type)
{
    // Parses all entities on the grid, and returns the grid position of
    // the nearest sought type. If not found, returns Vec3F(-1.0f).

    Vec3F target_pos(RG_MAX_WIDTH * 2, RG_MAX_HEIGHT * 2, RG_MAX_LENGTH * 2);
    Vec3F cur_best_distance = cur_pos - target_pos;
    
    for(uint x = 0; x < RG_MAX_WIDTH; x++)
    {
	for(uint y = 0; y < RG_MAX_HEIGHT; y++)
	{
	    for(uint z = 0; z < RG_MAX_LENGTH; z++)
	    {
		int id = room_grid.grid[x][y][z];
		if(id > -1)
		{
		    uint type = entities.types[id]; 
		    if(type == target_type)
		    {
			Vec3F pot_new_target = Vec3F((float)x, (float)y, (float)z);
			Vec3F new_distance = cur_pos - pot_new_target;
			if(!(pot_new_target == cur_pos) &&
			   magnitude(new_distance) < magnitude(cur_best_distance))
			{
			    target_pos        = pot_new_target;
			    cur_best_distance = cur_pos - target_pos;
			}
		    }
		}
	    }
	}
    }

    return target_pos;
}

void roomGridLookupInit(RoomGridLookup& rgl)
{
    for(uint i = 0; i < TOTAL_ROOMGRIDS; i++)
    {
	rgl.roomgrid_pointers[i] = NULL;
    }
}

// AI Functions
Vec3F aStarFindPath(RoomGrid& room_grid, Vec3F cur_grid_pos, Vec3F target_grid_pos)
{
    // TODO: Add exception handling if target is unreachable

    // TODO: room_grid and vector positions I'm using are not aligned - entities on 0 height,
    // but I am searching at height 1
    
    _assert(target_grid_pos.x >= 0 && target_grid_pos.x < RG_MAX_WIDTH);
    _assert(target_grid_pos.y >= 0 && target_grid_pos.y < RG_MAX_HEIGHT);
    _assert(target_grid_pos.z >= 0 && target_grid_pos.z < RG_MAX_LENGTH);

    std::vector<Node> open(RG_MAX_WIDTH * RG_MAX_HEIGHT);
    std::vector<Node> closed(RG_MAX_WIDTH * RG_MAX_HEIGHT);
    Node cur_cheapest_node(cur_grid_pos, cur_grid_pos, target_grid_pos, NULL);
    
    while(!(cur_cheapest_node.grid_pos == target_grid_pos))
    {
	// Add the starting or current cheapest node to closed
	closed.push_back(cur_cheapest_node);
	
        // Get open, walkable neighbors and update the node grid
        for(int x = -1; x < 2; x++)
        {
	    for(int z = -1; z < 2; z++)
	    {
		if((x == 0 && abs(z) == 1) || z == 0 && abs(x) == 1)
		{
		    Vec3F neighbor_offset((float)x, 0.0f, (float)z);
		    Vec3F node_pos = closed[closed.size() - 1].grid_pos + neighbor_offset;
		    Node  neighbor_node(cur_grid_pos,
					node_pos,
					target_grid_pos,
					&closed[closed.size() - 1]);
		    // If there is no entity occupying the node, add it to the node grid
		    // as a candidate
		    if(roomGridGetEntity(room_grid, node_pos) == NO_ENTITY || node_pos == target_grid_pos)
		    {
			// If the node pos is found in closed, don't add the node to open
			uint found = 0;
			for(uint i = 0; i < closed.size(); i++)
			{
			    if(closed[i].grid_pos == node_pos)
			    {
				found = 1;
				break;
			    }
			}

			if(!found)
			{
			    // If the node pos is found in open, and the new node is evaluated
			    // with a lower f cost then overwrite the entry's f cost
			    for(uint i = 0; i < open.size(); i++)
			    {
				if(open[i].grid_pos == neighbor_node.grid_pos)
				{
				    found = 1;
				    if(neighbor_node.f_cost < open[i].f_cost)
				    {
					open[i] = neighbor_node;
				    }
				}
			    }

			    // Otherwise, add it as a new open node
			    if(!found)
			    {
				open.push_back(neighbor_node);
			    }
			}
		    }
		}
	    }
        }

	// Find the next cheapest node to investigate
	// Reset the cheapest node for new search
	cur_cheapest_node = Node();
	uint cheapest_index = 0;
	for(uint i = 0; i < open.size(); i++)
	{
	    if(open[i].f_cost < cur_cheapest_node.f_cost ||
	       ((open[i].f_cost == cur_cheapest_node.f_cost) &&
		(open[i].h_cost < cur_cheapest_node.h_cost)))
	    {
		cur_cheapest_node = open[i];
		cheapest_index = i;
	    }
	    print(open[i].grid_pos);
	    std::cout << "\n";
	}
	std::cout<< "========= ^ Pre Removal ^ =============\n";
	open.erase(open.begin() + cheapest_index);
	for(uint i = 0; i < open.size(); i++)
	{
	    print(open[i].grid_pos);
	    std::cout << "\n";
	}
	std::cout<< "========== ^ Post Removal ^ ============\n";
    }

    // Walk back to the first node in the final path
    while(cur_cheapest_node.parent_p->parent_p != NULL)
    {
	cur_cheapest_node = *(cur_cheapest_node.parent_p);
    }
    Vec3F first_move = cur_cheapest_node.grid_pos;
    return first_move - cur_grid_pos;
}
