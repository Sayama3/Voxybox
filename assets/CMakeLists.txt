cmake_minimum_required(VERSION 3.24)

configure_file(textures/texture_checker.png textures/texture_checker.png COPYONLY)

configure_file(shaders/FlatColor_Frag.glsl shaders/FlatColor_Frag.glsl COPYONLY)
configure_file(shaders/FlatColor_VERT.glsl shaders/FlatColor_VERT.glsl COPYONLY)
configure_file(shaders/TextureShader.glsl shaders/TextureShader.glsl COPYONLY)
