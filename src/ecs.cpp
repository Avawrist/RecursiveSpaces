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
    pitch       = -37.0f;
    yaw         = 45.0f;
    n           = 0.8f;
    f           = 100.0f;
    fov         = 45.0f;
}

void cameraOffsetAngles(Camera& cam, float o_yaw, float o_pitch)
{
    cam.pitch -= o_pitch;
    cam.yaw   -= o_yaw;

    if (cam.yaw > 360.0f) {cam.yaw -= 360.0f;}
    if (cam.yaw < 0.0f) {cam.yaw += 360.0f;}
    cam.pitch = clamp(cam.pitch, -89.0f, 89.0f);
}

Mat4F cameraGetView(const Camera& cam, Vec3F cam_pos)
{
    // Assumes pitch & yaw are stored in degrees
    // Assumes RH coordinate system
    // Assumes Column major
    
    float pitch_rads = degToRads(cam.pitch);
    float yaw_rads   = degToRads(cam.yaw);
    
    float cos_pitch = cos(pitch_rads);
    float cos_yaw   = cos(yaw_rads);
    float sin_pitch = sin(pitch_rads);
    float sin_yaw   = sin(yaw_rads);

    Vec3F x_axis(cos_yaw, 0.0f, -sin_yaw);
    Vec3F y_axis(sin_yaw * sin_pitch, cos_pitch, cos_yaw * sin_pitch);
    Vec3F z_axis(sin_yaw * cos_pitch, -sin_pitch, cos_pitch * cos_yaw);

    return transpose(Mat4F(x_axis.x,   y_axis.x,   z_axis.x,   0.0f,
		 x_axis.y,   y_axis.y,   z_axis.y,   0.0f,
		 x_axis.z,   y_axis.z,   z_axis.z,   0.0f,
			   -dot(x_axis, cam_pos), -dot(y_axis, cam_pos), -dot(z_axis, cam_pos), 1.0f));
}

Mat4F cameraGetPerspective(const Camera& cam, float ar)
{
    // Assumes RH coordinate system
    // Assumes Column major
    
    float range        = cam.n - cam.f;
    float tan_half_fov = tan(degToRads(cam.fov) * 0.5f);

    return Mat4F(1.0f/(ar * tan_half_fov), 0.0f, 0.0f, 0.0f,                   // Row 1
	         0.0f, 1.0f/tan_half_fov, 0.0f, 0.0f,                              // Row 2
	         0.0f, 0.0f, (-cam.n - cam.f)/range, (2.0f * cam.f * cam.n)/range, // Row 3
	         0.0f, 0.0f, -1.0f, 0.0f);                                         // Row 4
    
}

Mat4F cameraGetOrthographic(const Camera& cam, int win_width, int win_height)
{
    // Assumes RH coordinate system
    // Assumes Column major

    float scaled_w = win_width / 10000.0f; // 10000
    float scaled_h = win_height / 10000.0f; // 10000
    
    // needs to match window aspect ratio
    float r = scaled_w / 2.0f;
    float l = -scaled_w / 2.0f;
    float t = scaled_h / 2.0f;
    float b = -scaled_h / 2.0f;
    float n = 1.0f;
    float f = 10.0f;
    /*
    return Mat4F(2.0f/(r - l), 0.0f,         0.0f,          -(r + l)/(r - l),
	         0.0f,         2.0f/(t - b), 0.0f,          -(t + b)/(t - b),
	         0.0f,         0.0f,         -2.0f/(f - n), -(f + n)/(f - n),
	         0.0f,         0.0f,          0.0f,          1.0f);
    */
    return Mat4F(1.0f/r, 0.0f,   0.0f,        0.0f,
	         0.0f,   1.0f/t, 0.0f,        0.0f,
	         0.0f,   0.0f,  -1.0f/25.0f, -1.0f,
	         0.0f,   0.0f,   0.0f,        1.0f);
}

///////////////////////////////
// Struct Component DirLight //
///////////////////////////////

DirLight::DirLight()
{
    color = Vec3F(1.0f, 1.0f, 1.0f);
    dir   = Vec3F(1.0f, -1.0f, 1.0f);
    ambient_strength = 0.5f;
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
    g_cost = MAX_WIDTH * MAX_LENGTH * 3;
    h_cost = MAX_WIDTH * MAX_LENGTH * 3;
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

LevelGrid::LevelGrid()
{
    memset(grid, -1, MAX_WIDTH * MAX_HEIGHT * MAX_LENGTH * sizeof(int));
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

    count = 0;
}

int activeEntitiesCreateEntity(ActiveEntities& entities, LevelGrid& level_grid,
			       Vec3F origin, uint entity_type)
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
	// if entity has a grid_position component, add to grid and set grid position
	if(entities.entity_templates.table[entity_type][COMPONENT_GRID_POSITION])
	{
	    levelGridSetEntity(level_grid, origin, entities.count);
	    entities.grid_positions[entities.count].position = origin;
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

void activeEntitiesRemoveInactives(ActiveEntities& entities, LevelGrid& level_grid)
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
		Vec3F inactive_grid_position = entities.grid_positions[i].position;
		levelGridRemoveEntity(level_grid, inactive_grid_position);
	    }
	    // If replacement entity is on the grid, update ID of last active
	    // entity in the grid, since it has changed
	    if(entities.entity_templates.table[entities.types[entities.count - 1]][COMPONENT_GRID_POSITION])
	    {
		Vec3F active_grid_position = entities.grid_positions[entities.count - 1].position;
		levelGridSetEntity(level_grid, active_grid_position, i);
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

/////////////////////////
// LevelGrid Functions //
/////////////////////////

int levelGridGetEntity(LevelGrid& level_grid, Vec3F pos)
{
    // Returns entity on success. Returns -1 if no entity. Returns -2 if out of bounds.
    
    // Check that target position is valid (within the grid)
    if(pos.x < 0.0f || pos.x >= MAX_WIDTH ||
       pos.y < 0.0f || pos.y >= MAX_HEIGHT ||
       pos.z < 0.0f || pos.z >= MAX_LENGTH)
    {
	return INVALID_RANGE;
    }

    return level_grid.grid[(int)pos.x][(int)pos.y][(int)pos.z];
}

void levelGridSetEntity(LevelGrid& level_grid, Vec3F pos, int entity_ID)
{
    _assert(pos.x >= 0.0f && pos.x < MAX_WIDTH);
    _assert(pos.y >= 0.0f && pos.y < MAX_HEIGHT);
    _assert(pos.z >= 0.0f && pos.z < MAX_LENGTH);
    
    level_grid.grid[(int)pos.x][(int)pos.y][(int)pos.z] = entity_ID;
}

void levelGridRemoveEntity(LevelGrid& level_grid, Vec3F pos)
{
    _assert(pos.x >= 0.0f && pos.x < MAX_WIDTH);
    _assert(pos.y >= 0.0f && pos.y < MAX_HEIGHT);
    _assert(pos.z >= 0.0f && pos.z < MAX_LENGTH);
    
    level_grid.grid[(int)pos.x][(int)pos.y][(int)pos.z] = -1;
}

Vec3F levelGridFindNearestType(LevelGrid& level_grid, ActiveEntities& entities,
			       Vec3F cur_pos, uint target_type)
{
    // Parses all entities on the grid, and returns the grid position of
    // the nearest sought type. If not found, returns Vec3F(-1.0f).

    Vec3F target_pos(MAX_WIDTH * 2, MAX_HEIGHT * 2, MAX_LENGTH * 2);
    Vec3F cur_best_distance = cur_pos - target_pos;
    
    for(uint x = 0; x < MAX_WIDTH; x++)
    {
	for(uint y = 0; y < MAX_HEIGHT; y++)
	{
	    for(uint z = 0; z < MAX_LENGTH; z++)
	    {
		int id = level_grid.grid[x][y][z];
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

// AI Functions
Vec3F aStarFindPath(LevelGrid& level_grid, Vec3F cur_grid_pos, Vec3F target_grid_pos)
{
    // TODO: Add exception handling if target is unreachable

    // TODO: Level_grid and vector positions I'm using are not aligned - entities on 0 height,
    // but I am searching at height 1
    
    _assert(target_grid_pos.x >= 0 && target_grid_pos.x < MAX_WIDTH);
    _assert(target_grid_pos.y >= 0 && target_grid_pos.y < MAX_HEIGHT);
    _assert(target_grid_pos.z >= 0 && target_grid_pos.z < MAX_LENGTH);

    std::vector<Node> open(MAX_WIDTH * MAX_HEIGHT);
    std::vector<Node> closed(MAX_WIDTH * MAX_HEIGHT);
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
		    if(levelGridGetEntity(level_grid, node_pos) == NO_ENTITY || node_pos == target_grid_pos)
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
