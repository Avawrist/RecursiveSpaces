// ================================================
// Title: component.cpp
// Description: The source file component structs
// ================================================

#include "component.hpp"

////////////////////////////////
// Struct Component Transform //
////////////////////////////////

Transform::Transform()
{
    x_scale = 1.0f;
    y_scale = 1.0f;
    z_scale = 1.0f;
}

// ActiveEntities Function Prototypes

void activeEntitiesSetRenderPointers(AssetManager& asset_manager, ShaderManager& shader_manager,
                                     Render& render_comp, uint entity_type)
{
    // Mesh 01
    render_comp.mesh_01_p   = (Mesh*)assetManagerGetAssetP(asset_manager, entity_type, MESH01, 0);
    // Diffuse Texture
    render_comp.texture_d_p = (Texture*)assetManagerGetAssetP(asset_manager, entity_type, TEXTURE_D, 0);
    // Normal Texture
    render_comp.texture_n_p = (Texture*)assetManagerGetAssetP(asset_manager, entity_type, TEXTURE_N, 0);
    // Specular Texture
    render_comp.texture_s_p = (Texture*)assetManagerGetAssetP(asset_manager, entity_type, TEXTURE_S, 0);
    // Shader | TODO: Merge shader manager code into asset manager?
    render_comp.shader_p    = (Shader*)shaderManagerGetShaderP(shader_manager, BLINNPHONG);
}

void activeEntitiesSetSFXPointers(AssetManager& asset_manager, void* sound_interface_p,
                                    SFX& sound_comp, uint entity_type)
{
    // Sound 01
    sound_comp.sound_01_p = (Sound*)assetManagerGetAssetP(asset_manager, entity_type,
							  SOUND01, sound_interface_p);
    // Sound 02
    sound_comp.sound_02_p = (Sound*)assetManagerGetAssetP(asset_manager, entity_type,
							  SOUND02, sound_interface_p);
    // Sound 03
    sound_comp.sound_03_p = (Sound*)assetManagerGetAssetP(asset_manager, entity_type,
							  SOUND03, sound_interface_p);
}

int activeEntitiesCreateEntity(ActiveEntities& entities, AssetManager& asset_manager,
			       ShaderManager& shader_manager, Vec3F origin, uint type, void* sound_interface_p)
{
    // Returns entity ID on success, -1 on failure
    
    _assert(type >= 0 && type < TOTAL_ENTITY_TYPES);

    // Parse all entities and find next open slot in array:
    for(uint i = 0; i < MAX_ENTITIES; i++)
    {
	if(entities.mask[i] == COMPONENT_NONE)
	{
	    // TODO: Get entity mask using type, record mask in entities (line below is temp)
	    entities.mask[i] = COMPONENT_TRANSFORM | COMPONENT_RENDER | COMPONENT_SFX; 
	    // Record type in entities
	    entities.type[i] = type;
	    // Set transform position with origin
	    if((entities.mask[i] & std::bitset<MAX_COMPONENTS>(COMPONENT_TRANSFORM)) == COMPONENT_TRANSFORM)
	    {
		entities.transform[i].position = origin;
	    }
	    // Set render asset pointers
	    if((entities.mask[i] & std::bitset<MAX_COMPONENTS>(COMPONENT_RENDER)) == COMPONENT_RENDER)
	    {
	        activeEntitiesSetRenderPointers(asset_manager, shader_manager, entities.render[i], type);
	    }
	    // Set sound asset pointers
	    if((entities.mask[i] & std::bitset<MAX_COMPONENTS>(COMPONENT_SFX)) == COMPONENT_SFX)
	    {
	        activeEntitiesSetSFXPointers(asset_manager, sound_interface_p, entities.sfx[i], type);
	    }
	    
	    return i;
	}
    }

    return -1;
}

void activeEntitiesRemoveEntity(ActiveEntities& entities, int entity_ID)
{
    _assert(entity_ID >= 0 && entity_ID < MAX_ENTITIES);
    
    // Remove components from entity, system knows the entity is free to reassign/overwrite
    entities.mask[entity_ID] = COMPONENT_NONE;
}

// Component Function Prototypes
Mat4F transformGetModel(Transform& transform)
{
    Mat4F model = Mat4F(transform.x_scale, 0.0f, 0.0f, 0.0f,
	                0.0f, transform.y_scale, 0.0f, 0.0f,
	                0.0f, 0.0f, transform.z_scale, 0.0f,
	                transform.position.x, transform.position.y, transform.position.z, 1.0f);
    return model;
}

// Component Update Functions

void activeEntitiesRender(ActiveEntities& entities, GameWindow& game_window, FrameTexture& framebuffer)
{
    //////////////////
    // Set GL state //
    //////////////////
    glViewport(0, 0, game_window.view_width, game_window.view_height);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.fbo);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ////////////////////////////
    // Update Shader Uniforms //
    ////////////////////////////

    // Update Blinn-Phong Shader Uniforms

    // Update Grid Shader Uniforms
    
    ////////////////////////////////////////
    // Render Entites w/ Render Component //
    ////////////////////////////////////////
    for(uint i = 0; i < MAX_ENTITIES; i++)
    {
	if((entities.mask[i] & std::bitset<MAX_COMPONENTS>(COMPONENT_RENDER)) == COMPONENT_RENDER)
	{
	    // Set Shader
	    glUseProgram(entities.render[i].shader_p->program_id);
	    // Update Model Uniform in Shader
	    Mat4F model = transformGetModel(entities.transform[i]);
	    shaderAddMat4Uniform(entities.render[i].shader_p, "model", model.getPointer());
            // Bind Diffuse Texture
	    glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D, entities.render[i].texture_d_p->texture_id);
	    // Bind Normal Texture
	    glActiveTexture(GL_TEXTURE1);
	    glBindTexture(GL_TEXTURE_2D, entities.render[i].texture_n_p->texture_id);
	    // Bind Mesh
	    glBindVertexArray(entities.render[i].mesh_01_p->vao);
	    // Draw
	    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)entities.render[i].mesh_01_p->data.size());
	}
    }
}
