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

Mat4F transformGetModel(Transform& transform)
{
    Mat4F model = Mat4F(transform.x_scale, 0.0f, 0.0f, 0.0f,
	                0.0f, transform.y_scale, 0.0f, 0.0f,
	                0.0f, 0.0f, transform.z_scale, 0.0f,
	                transform.position.x, transform.position.y, transform.position.z, 1.0f);
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

    return Mat4F(x_axis.x,   y_axis.x,   z_axis.x,   0.0f,
		 x_axis.y,   y_axis.y,   z_axis.y,   0.0f,
		 x_axis.z,   y_axis.z,   z_axis.z,   0.0f,
		 -dot(x_axis, cam_pos), -dot(y_axis, cam_pos), -dot(z_axis, cam_pos), 1.0f);
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

    float scaled_w = win_width / 10000.0f;
    float scaled_h = win_height / 10000.0f;
    
    // needs to match window aspect ratio
    float r = scaled_w / 2.0f;
    float l = -scaled_w / 2.0f;
    float t = scaled_h / 2.0f;
    float b = -scaled_h / 2.0f;
    float n = 0.0f;
    float f = 10.0f;

    return Mat4F(2.0f/(r - l), 0.0f,         0.0f,          -(r + l)/(r - l),
	         0.0f,         2.0f/(t - b), 0.0f,          -(t + b)/(t - b),
	         0.0f,         0.0f,         -2.0f/(f - n), -(f + n)/(f - n),
	         0.0f,         0.0f,          0.0f,          1.0f);
}

///////////////////////////////
// Struct Component DirLight //
///////////////////////////////

DirLight::DirLight()
{
    color = Vec3F(1.0f, 1.0f, 1.0f);
    dir   = Vec3F(0.0f, -1.0f, -1.0f);
    ambient_strength = 0.25f;
}

////////////////////////////
// Struct EntityTemplates //
////////////////////////////

EntityTemplates::EntityTemplates()
{
    // Initialize all template components to 0
    memset(table, 0, TOTAL_ENTITY_TYPES * TOTAL_COMPONENT_TYPES * sizeof(uint));
}

//////////////////////////////
// ActiveEntities Functions //
//////////////////////////////

ActiveEntities::ActiveEntities()
{
    // Initialize all types to 0 (meaning no type, vacancy)
    memset(type, NONE, MAX_ENTITIES * sizeof(uint));

    count = 0;
}

int activeEntitiesCreateEntity(ActiveEntities& entities, Vec3F origin, uint entity_type)
{
    // Returns entity ID on success, -1 on failure
    
    _assert(entity_type >= 0 && entity_type < TOTAL_ENTITY_TYPES);

    if (entities.count < MAX_ENTITIES)
    {
	entities.type[entities.count] = entity_type;
	entities.count++;
	// sets transform even if never used
	entities.transform[entities.count] = Vec3F(origin);
	return entities.count;
    }
    OutputDebugStringA("ERROR - Failed to create entity - Max entities reached.\n");
    return -1;
}

void activeEntitiesMarkInactive(ActiveEntities& entities, uint entity_ID)
{
    _assert(entity_ID >= 0 && entity_ID < entities.count);
    
    // Sets type to -1, system knows the entity is free to overwrite
    entities.type[entity_ID] = NONE;
}

void activeEntitiesRemoveInactives(ActiveEntities& entities)
{
    // Should be run after all other entity updates. Searches for inactive entities,
    // if found, overwrites the inactive with the active entity on the end of the arrays,
    // then decreases active_count by 1.
    // Repeats until the type array is completely searched.

    for(int i = entities.count - 1; i >= 0; i--)
    {
	if(entities.type[i] == NONE)
	{
	    // Copy data from last active entity and fill inactive slot 
	    entities.type[i]      = entities.type[entities.count - 1];
	    entities.transform[i] = entities.transform[entities.count - 1];
	    entities.camera[i]    = entities.camera[entities.count - 1];
	    entities.dir_light[i] = entities.dir_light[entities.count - 1];
	    // Set last active entity type to NONE for memory readability
	    entities.type[entities.count - 1] = NONE;
	    // Decrease entity count by one, effectively removing the inactive entity
	    // while preserving the entity on the end of the arrays. 
	    entities.count--;
	}
    }
}



