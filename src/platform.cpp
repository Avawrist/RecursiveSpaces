// ====================================================================================
// Title: platform.cpp
// Description: The source file for the platform layer, abstracting the
//              platform dependent APIs
// ====================================================================================

#include "platform.hpp"

int platformInitAPIs(GameWindow& game_window, uint width, uint height)
{
    // Returns 1 on success, 0 on failure
    
    // Initialize GLFW (Enumerates windows, joysticks, starts the timer)
    if(!platformInitGLFW()) {return 0;}

    // Initialize window
    if(!platformInitWindow(game_window, width, height, "First Game")) {return 0;}

    // Load OpenGL Functions & Extensions (Must be called after window creation)
    if(!platformInitOpenGL()) {return 0;}

    // Load XAudio2 library & init required COM library
    if(!platformLoadXAudio2()) {return 0;}
    CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
    
    return 1;
}

int platformInitGLFW()
{
    // Init GLFW library
    if(!glfwInit())
    {
	OutputDebugStringA("ERROR: Failed to initialize GLFW\n");
	return 0;
    }

    // Set context Hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // TO-DO: Remove on release
    
    return 1;
}

int platformInitOpenGL()
{
    // Load OpenGL procedures through GLAD
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
	OutputDebugStringA("ERROR: Glad failed to load the OpenGL functions/extensions\n");
	return 0;
    }

    // Set initial OpenGL state
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.15f, 0.19f, 0.45f, 1.0f);
    // Cull Faces
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    // Debug - TO-DO: Remove following lines for release build
    glEnable(GL_DEBUG_OUTPUT); 
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    
    return 1;
}

int platformLoadXAudio2()
{
    // Returns 1 on success, 0 on failure
    
    // Windows 10 
    HMODULE x_audio_2_libs = LoadLibraryA("xaudio2_9.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_9.dll.\n");
	return 1;
    }

    x_audio_2_libs = LoadLibraryA("xaudio2_9redist.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_9redist.dll.\n");
	return 1;
    }

    // Windows 8
    x_audio_2_libs = LoadLibraryA("xaudio2_8.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_8.dll.\n");
	return 1;
    }

    x_audio_2_libs = LoadLibraryA("xaudio2_8redist.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_8redist.dll.\n");
	return 1;
    }

    // DirectX SDK
    x_audio_2_libs = LoadLibraryA("xaudio2_7.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_7.dll.\n");
	return 1;
    }

    x_audio_2_libs = LoadLibraryA("xaudio2_7redist.dll");
    if(x_audio_2_libs)
    {
	OutputDebugStringA("SUCCESS: Loaded xaudio2_7redist.dll.\n");
	return 1;
    }
    
    return 0;
}

int platformInitWindow(GameWindow& game_window, uint _width, uint _height, c_char* name)
{
    // Returns 1 on success, 0 on failure
    
    /////////////////
    // Init Values //
    /////////////////
    // Set window measurements
    game_window.win_width   = _width;
    game_window.win_height  = _height;
    game_window.view_width  = (uint)(game_window.win_width * 1.0f);
    game_window.view_height = (uint)(game_window.win_height * 1.0f);
    game_window.x_center    = game_window.win_width * 0.5f;
    game_window.y_center    = game_window.win_height * 0.5f;
    game_window.win_ar      = (float)game_window.win_width / (float)game_window.win_height;
    // Set close condition
    game_window.close = false;
    // Set time fields
    game_window.target_framerate = 0;

    /////////////////////////////
    // Get framerate from GLFW //
    /////////////////////////////
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if(monitor)
    {
	// Get vidmode struct from GLFW, and read the refresh rate from it
	const GLFWvidmode* vid_mode = glfwGetVideoMode(monitor);
	if(vid_mode)
	{
	    game_window.target_framerate = vid_mode->refreshRate;
	    char msg[256];
	    sprintf_s(msg, "Target framerate found: %i\n", game_window.target_framerate);
	    OutputDebugStringA(msg);
	}
    }
    // If framerate not found, set to 60 by default
    if(game_window.target_framerate == 0) {game_window.target_framerate = 60;} 
    game_window.target_cycle_length_secs = 1.0f / game_window.target_framerate;  
    game_window.cycle_start_time_secs    = 0.0f;
    game_window.d_time                   = 0;

    ///////////////////////////////////
    // Set sleep granularity to 1 MS //
    ///////////////////////////////////
    game_window.sleep_is_granular = (timeBeginPeriod(1) == TIMERR_NOERROR);
    
    /////////////////////////////
    // Create window & context //
    /////////////////////////////
    game_window.window_p = (void*)glfwCreateWindow(game_window.win_width, game_window.win_height,
						   name, NULL, NULL);
    outputGLFWError();
    if(game_window.window_p)
    {
	// Make new window the current context	
	glfwMakeContextCurrent((GLFWwindow*)game_window.window_p);
	outputGLFWError();
	
	// Disable cursor
	glfwSetInputMode((GLFWwindow*)game_window.window_p, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	outputGLFWError();

	return 1;
    }
    else
    {
	OutputDebugStringA("ERROR: GLFW failed to create window/context\n");
	return 0;
    }
}

void platformFreeWindow(GameWindow& game_window)
{
    glfwDestroyWindow((GLFWwindow*)game_window.window_p);
    glfwTerminate(); // This will stop all GLFW functions including timer and input
}

void platformSwapBuffers(GameWindow& game_window)
{
    double time_elapsed_this_cycle_secs = glfwGetTime() - game_window.cycle_start_time_secs;

    // While the cycle is early
    while(time_elapsed_this_cycle_secs < game_window.target_cycle_length_secs)
    {
	// Wait for the difference of time between the current cycle time and the target time
	if(game_window.sleep_is_granular)
	{
	    DWORD sleep_time_ms = (DWORD)(1000.0f * (game_window.target_cycle_length_secs -
						     time_elapsed_this_cycle_secs));
	    Sleep(sleep_time_ms);
	    time_elapsed_this_cycle_secs = glfwGetTime() - game_window.cycle_start_time_secs;
	}
	else // Only cycle spin if sleep granularity wasn't set. 
	{
	    time_elapsed_this_cycle_secs = glfwGetTime() - game_window.cycle_start_time_secs;
	}
    }
    // TODO: Handle case where cycle takes LONGER than the target cycle length
    
    // Swap buffers
    glfwSwapBuffers((GLFWwindow*)game_window.window_p);
    outputGLFWError();

    // Update delta time
    game_window.d_time = (float)time_elapsed_this_cycle_secs;

    char msg2[256];
    sprintf_s(msg2, "Delta time from previous frame: %f\n", game_window.d_time);
    OutputDebugStringA(msg2);

    // Reset counters
    game_window.cycle_start_time_secs = glfwGetTime();
}

double platformGetTime()
{
    return glfwGetTime();
}

void platformGetInputsThisFrame(InputManager &im, GameWindow &gw)
{
    glfwPollEvents(); // Processes all input events that occurred this cycle
    
    // Populate all inputs received this cycle into inputs_on_frame[1_FRAME_PRIOR]
    // Will be processed in the game layer next cycle
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_UP]    = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_UP);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_DOWN]  = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_DOWN);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_LEFT]  = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_LEFT);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_ARROW_RIGHT] = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_RIGHT);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_SPACE]       = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_SPACE);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_ESC]         = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_ESCAPE);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_W]           = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_W);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_S]           = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_S);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_A]           = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_A);
    im.inputs_on_frame[FRAME_1_PRIOR][KEY_D]           = glfwGetKey((GLFWwindow*)gw.window_p, GLFW_KEY_D);

    // Get cursor input
    glfwGetCursorPos((GLFWwindow*)gw.window_p, &im.cursor.x_pos, &im.cursor.y_pos);
}

void platformRenderShadowMapToScreen(AssetManager& asset_manager,
				     GameWindow& game_window,
				     FrameTexture& depth_framebuffer)
{
    Shader* sm_debug_shader_p = (Shader*)assetManagerGetShaderP(asset_manager, SMDEBUG);
    glViewport(0, 0, game_window.win_width, game_window.win_height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glUseProgram(sm_debug_shader_p->program_id);
    shaderAddIntUniform(sm_debug_shader_p, "depth_map", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depth_framebuffer.depth_text_id);
    glBindVertexArray(depth_framebuffer.quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);    
}

void platformRenderShadowMapToBuffer(ActiveEntities& active_entities,
				     const FrameTexture& depth_framebuffer,
				     const RoomGrid& room,
				     AssetManager& asset_manager,
				     const GameWindow& game_window,
				     uint dir_light_id)
{
    //////////////////
    // Set GL state //
    //////////////////
    glViewport(0, 0, depth_framebuffer.width, depth_framebuffer.height);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_framebuffer.fbo);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    
    /////////////////
    // Prep Shader //
    /////////////////
    Shader* shadowmap_shader_p = (Shader*)assetManagerGetShaderP(asset_manager, SHADOWMAP);
    glUseProgram(shadowmap_shader_p->program_id);

    // View Mat
    Mat4F view = lookAt(active_entities.transforms[dir_light_id].position,
		        active_entities.dir_lights[dir_light_id].target,
			Vec3F(0.0f, 1.0f, 0.0f));
    shaderAddMat4Uniform(shadowmap_shader_p, "view", view.getPointer());

    // Projection Mat
    float ortho_height = 20.0f;
    float ortho_width  = ortho_height * game_window.win_ar;
    Mat4F projection = getOrthoProjection(-ortho_width * 0.5f,
					   ortho_width * 0.5f,
					  -ortho_height * 0.5f,
					   ortho_height * 0.5f,
					   0.05f,
					  ortho_height * 3.0f);
    shaderAddMat4Uniform(shadowmap_shader_p, "projection", projection.getPointer());

    //////////////////////////
    // Render Entity Depths //
    //////////////////////////
    for(uint i = 0; i < active_entities.count; i++)
    {
	if(active_entities.entity_templates.table[active_entities.types[i]][COMPONENT_RENDER] &&
	   active_entities.entity_templates.table[active_entities.types[i]][COMPONENT_TRANSFORM])
	{
	    // Update transform scale and get model -
	    // TODO: Move this up into update loop
	    active_entities.transforms[i].x_scale = room.current_scale;
	    active_entities.transforms[i].y_scale = room.current_scale;
	    active_entities.transforms[i].z_scale = room.current_scale;

	    // TODO: Need to translate also, not just scale for zoom effect to work
	    
	    Mat4F model = transformGetModel(active_entities.transforms[i]);
	    shaderAddMat4Uniform(shadowmap_shader_p, "model", model.getPointer());
	    Mesh* mesh_01_p = (Mesh*)assetManagerGetAssetP(asset_manager,
							   active_entities.types[i],
							   MESH01,
							   0);
	    glBindVertexArray(mesh_01_p->vao);
	    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh_01_p->data.size());
	}
    }
}

void platformRenderEntitiesToBuffer(const ActiveEntities& active_entities,
				    const FrameTexture& framebuffer,
				    const FrameTexture& depth_framebuffer,
				    const RoomGrid& room,
				    const GameWindow& game_window,
				    AssetManager& asset_manager,
				    uint dir_light_id,
				    uint cam_id)
{
    //////////////////
    // Set GL state //
    //////////////////
    glViewport(0, 0, framebuffer.width, framebuffer.height);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.fbo);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);
    
    /////////////////////////
    // Set Shader Uniforms //
    /////////////////////////
    Shader* bp_shader_p = (Shader*)assetManagerGetShaderP(asset_manager, BLINNPHONG);
    glUseProgram(bp_shader_p->program_id);
    // Light View Mat
    Mat4F light_view = lookAt(active_entities.transforms[dir_light_id].position,
			      active_entities.cameras[cam_id].target,
			      Vec3F(0.0f, 1.0f, 0.0f));
    shaderAddMat4Uniform(bp_shader_p, "light_view", light_view.getPointer());    
    // Cam View Mat
    Mat4F cam_view = lookAt(active_entities.transforms[cam_id].position,
			    active_entities.cameras[cam_id].target,
			    Vec3F(0.0f, 1.0f, 0.0f));
    shaderAddMat4Uniform(bp_shader_p, "cam_view", cam_view.getPointer());
    // Projection Mat
    float ortho_height = 20.0f;
    float ortho_width = ortho_height * game_window.win_ar;
    Mat4F projection = getOrthoProjection(-ortho_width * 0.5f,
					   ortho_width * 0.5f,
					  -ortho_height * 0.5f,
					   ortho_height * 0.5f,
					   0.05f,
					   ortho_height * 3.0f);
    shaderAddMat4Uniform(bp_shader_p, "projection", projection.getPointer());
    // Cam Pos
    shaderAddVec3Uniform(bp_shader_p, "cam_pos", active_entities.transforms[cam_id].position);
    // Single Dir Light
    shaderAddVec3Uniform(bp_shader_p,
			 "dirLight.color",
			 active_entities.dir_lights[dir_light_id].color);
    shaderAddVec3Uniform(bp_shader_p,
			 "dirLight.dir",
			 active_entities.dir_lights[dir_light_id].dir);
    shaderAddFloatUniform(bp_shader_p,
			  "dirLight.ambient_strength",
			  active_entities.dir_lights[dir_light_id].ambient_strength);
    // Textures
    shaderAddIntUniform(bp_shader_p, "diffuse_map", 0);
    shaderAddIntUniform(bp_shader_p, "normal_map", 1);
    shaderAddIntUniform(bp_shader_p, "spec_map", 2);
    shaderAddIntUniform(bp_shader_p, "shadow_map", 3);
    
    ///////////////////////////////
    // Render Entities to Buffer //
    ///////////////////////////////
    for(uint i = 0; i < active_entities.count; i++)
    {
	if(active_entities.entity_templates.table[active_entities.types[i]][COMPONENT_RENDER] &&
	   active_entities.entity_templates.table[active_entities.types[i]][COMPONENT_TRANSFORM])
	{
	    // Mesh 01
	    Mesh* mesh_01_p  = (Mesh*)assetManagerGetAssetP(asset_manager,
							    active_entities.types[i],
							    MESH01,
							    0);
	    // Diffuse Texture
	    Texture* texture_d_p = (Texture*)assetManagerGetAssetP(asset_manager,
								   active_entities.types[i],
								   TEXTURE_D,
								   0);
	    // Normal Texture
	    Texture* texture_n_p = (Texture*)assetManagerGetAssetP(asset_manager,
								   active_entities.types[i],
								   TEXTURE_N,
								   0);
	    // Specular Texture
	    Texture* texture_s_p = (Texture*)assetManagerGetAssetP(asset_manager,
								   active_entities.types[i],
								   TEXTURE_S,
								   0);
	    
	    // Update Model Uniform in Shader
	    Mat4F model = transformGetModel(active_entities.transforms[i]);
	    shaderAddMat4Uniform(bp_shader_p, "model", model.getPointer());
	    // Bind Diffuse Texture
	    glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D, texture_d_p->texture_id);
	    // Bind Normal Texture
	    glActiveTexture(GL_TEXTURE1);
	    glBindTexture(GL_TEXTURE_2D, texture_n_p->texture_id);
	    // Bind Specular Texture
	    glActiveTexture(GL_TEXTURE2);
	    glBindTexture(GL_TEXTURE_2D, texture_s_p->texture_id);
	    // Bind Shadow Map Texture
	    glActiveTexture(GL_TEXTURE3);
	    glBindTexture(GL_TEXTURE_2D, depth_framebuffer.depth_text_id);
	    // Bind Mesh
	    glBindVertexArray(mesh_01_p->vao);
	    // Draw
	    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh_01_p->data.size());
	}
    }
}

void platformRenderDebugElementsToBuffer(const GameWindow& game_window,
					 AssetManager& asset_manager,
					 Vec3F cam_pos,
					 Vec3F cam_target,
					 DebugGrid* grid_p)
{
    //////////////////////////////
    // Set Grid Shader Uniforms //
    //////////////////////////////
    Shader* grid_shader_p = (Shader*)assetManagerGetShaderP(asset_manager, DB_GRID);
    glUseProgram(grid_shader_p->program_id);
    // Model
    Mat4F grid_model = Mat4F(1.0f);
    shaderAddMat4Uniform(grid_shader_p, "model", grid_model.getPointer());
    // View
    Mat4F view = lookAt(cam_pos, cam_target, Vec3F(0.0f, 1.0f, 0.0f));
    shaderAddMat4Uniform(grid_shader_p, "view", view.getPointer());
    // Projection
    float ortho_height = 20.0f;
    float ortho_width = ortho_height * game_window.win_ar;
    Mat4F projection = getOrthoProjection(-ortho_width * 0.5f,
					   ortho_width * 0.5f,
					  -ortho_height * 0.5f,
					   ortho_height * 0.5f,
					   0.05f,
					   ortho_height * 3.0f);
    shaderAddMat4Uniform(grid_shader_p, "projection", projection.getPointer());

    ///////////////////////////
    // Render Debug Elements //
    ///////////////////////////
    debugGridDraw(grid_p, grid_shader_p, Vec3F(0.86f, 0.65f, 0.13f), 1.0f);
}

void platformBlitBufferToBuffer(const FrameTexture& source_framebuffer,
				const FrameTexture& target_framebuffer,
				uint src_x0, uint src_y0,
				uint src_x1, uint src_y1,
				uint dst_x0, uint dst_y0,
				uint dst_x1, uint dst_y1)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, source_framebuffer.fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target_framebuffer.fbo);
    glBlitFramebuffer(src_x0,
		      src_y0,
		      src_x1,
		      src_y1,
		      dst_x0,
		      dst_y0,
		      dst_x1,
		      dst_y1,
		      GL_COLOR_BUFFER_BIT,
		      GL_NEAREST);

}

void platformRenderPP(AssetManager& asset_manager,
		      const FrameTexture& non_msaa_framebuffer)
{
    //////////////////
    // Set GL State //
    //////////////////
    glViewport(0, 0, non_msaa_framebuffer.width, non_msaa_framebuffer.height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
    glClear(GL_COLOR_BUFFER_BIT); 
    glDisable(GL_DEPTH_TEST); // Disable so quad is visible

    /////////////////////////
    // Set Shader Uniforms //
    /////////////////////////
    Shader* pp_shader_p = (Shader*)assetManagerGetShaderP(asset_manager, POSTPROCESS);
    glUseProgram(pp_shader_p->program_id); // Set post-process shader
    shaderAddIntUniform(pp_shader_p, "color_texture", 0); // Update uniform
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, non_msaa_framebuffer.color_text_id); // Bind color texture to pp shader
    glBindVertexArray(non_msaa_framebuffer.quad_vao); // Bind quad vao

    ///////////////////////
    // Render PP Texture //
    ///////////////////////
    glDrawArrays(GL_TRIANGLES, 0, 6); // 6 vertices in the quad data
    glEnable(GL_DEPTH_TEST);
}

int platformLoadEntityTemplatesFromTxt(ActiveEntities& active_entities, c_char* path)
{
    // Returns 1 on success, 0 on failure

    FILE* file_p = NULL;
    fopen_s(&file_p, path, "r");
    if(!file_p) {return 0;}

    char  next[256];
    int   entity_type;
    int   component_type;
    
    do
    {
	// Get first string
	fscanf_s(file_p, "%s", next, 256);
	if(next[0] != '#')
	{
	    // Read entity type
	    fscanf_s(file_p, "%i", &entity_type);
	    // Eat hyphen
	    fscanf_s(file_p, "%s", next, 256);
	    do
	    {
		// Get component type
		fscanf_s(file_p, "%i", &component_type);
		if(component_type != -1) {active_entities.entity_templates.table[entity_type][component_type] = 1;}
	    } while(component_type != -1);
	}
	else
	{
	    // Eat line
	    fgets(next, 512, file_p);
	}
    } while(!feof(file_p)); 
    fclose(file_p);

    return 1;
}

/////////////////////
// Debug Functions //
/////////////////////

void outputGLFWError()
{
    c_char* errorMsg;
    int errorCode = glfwGetError(&errorMsg);
    if(errorMsg)
    {
	char code_buf[256];
	sprintf_s(code_buf, "ERROR CODE: %i", errorCode);
	OutputDebugStringA(code_buf);
        OutputDebugStringA(errorMsg);
	OutputDebugStringA("\n");
    }
}

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, 
                       GLsizei length, c_char *message, c_void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    OutputDebugStringA("---------------\n");
    OutputDebugStringA("Debug message:\n");

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             OutputDebugStringA("Source: API"); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   OutputDebugStringA("Source: Window System"); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: OutputDebugStringA("Source: Shader Compiler"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     OutputDebugStringA("Source: Third Party"); break;
        case GL_DEBUG_SOURCE_APPLICATION:     OutputDebugStringA("Source: Application"); break;
        case GL_DEBUG_SOURCE_OTHER:           OutputDebugStringA("Source: Other"); break;
    } OutputDebugStringA("\n");

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               OutputDebugStringA("Type: Error"); break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: OutputDebugStringA("Type: Deprecated Behaviour"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  OutputDebugStringA("Type: Undefined Behaviour"); break; 
        case GL_DEBUG_TYPE_PORTABILITY:         OutputDebugStringA("Type: Portability"); break;
        case GL_DEBUG_TYPE_PERFORMANCE:         OutputDebugStringA("Type: Performance"); break;
        case GL_DEBUG_TYPE_MARKER:              OutputDebugStringA("Type: Marker"); break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          OutputDebugStringA("Type: Push Group"); break;
        case GL_DEBUG_TYPE_POP_GROUP:           OutputDebugStringA("Type: Pop Group"); break;
        case GL_DEBUG_TYPE_OTHER:               OutputDebugStringA("Type: Other"); break;
    } OutputDebugStringA("\n");
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         OutputDebugStringA("Severity: high"); break;
        case GL_DEBUG_SEVERITY_MEDIUM:       OutputDebugStringA("Severity: medium"); break;
        case GL_DEBUG_SEVERITY_LOW:          OutputDebugStringA("Severity: low"); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: OutputDebugStringA("Severity: notification"); break;
    } OutputDebugStringA("\n");
    OutputDebugStringA("\n");
}
