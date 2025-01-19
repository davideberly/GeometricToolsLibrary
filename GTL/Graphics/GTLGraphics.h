// Geometric Tools Library
// https://www.geometrictools.com
// Copyright (c) 2025 Geometric Tools LLC
// Distributed under the Boost Software License, Version 1.0
// https://www.boost.org/LICENSE_1_0.txt
// File Version: 2025.01.19

#pragma once

// Base
#include <GTL/Graphics/Base/BaseEngine.h>
#include <GTL/Graphics/Base/GEDrawTarget.h>
#include <GTL/Graphics/Base/GEInputLayoutManager.h>
#include <GTL/Graphics/Base/GEObject.h>
#include <GTL/Graphics/Base/Graphics.h>
#include <GTL/Graphics/Base/GraphicsEngine.h>
#include <GTL/Graphics/Base/GraphicsObject.h>

// Effects
#include <GTL/Graphics/Effects/AmbientLightEffect.h>
#include <GTL/Graphics/Effects/AreaLightEffect.h>
#include <GTL/Graphics/Effects/BumpMapEffect.h>
#include <GTL/Graphics/Effects/ConstantColorEffect.h>
#include <GTL/Graphics/Effects/CubeMapEffect.h>
#include <GTL/Graphics/Effects/DirectionalLightEffect.h>
#include <GTL/Graphics/Effects/DirectionalLightTextureEffect.h>
#include <GTL/Graphics/Effects/Font.h>
#include <GTL/Graphics/Effects/FontArialW400H12.h>
#include <GTL/Graphics/Effects/FontArialW400H14.h>
#include <GTL/Graphics/Effects/FontArialW400H16.h>
#include <GTL/Graphics/Effects/FontArialW400H18.h>
#include <GTL/Graphics/Effects/FontArialW700H12.h>
#include <GTL/Graphics/Effects/FontArialW700H14.h>
#include <GTL/Graphics/Effects/FontArialW700H16.h>
#include <GTL/Graphics/Effects/FontArialW700H18.h>
#include <GTL/Graphics/Effects/GlossMapEffect.h>
#include <GTL/Graphics/Effects/LightCameraGeometry.h>
#include <GTL/Graphics/Effects/LightEffect.h>
#include <GTL/Graphics/Effects/Lighting.h>
#include <GTL/Graphics/Effects/Material.h>
#include <GTL/Graphics/Effects/OverlayEffect.h>
#include <GTL/Graphics/Effects/PlanarShadowEffect.h>
#include <GTL/Graphics/Effects/PlanarReflectionEffect.h>
#include <GTL/Graphics/Effects/PointLightEffect.h>
#include <GTL/Graphics/Effects/PointLightTextureEffect.h>
#include <GTL/Graphics/Effects/ProjectedTextureEffect.h>
#include <GTL/Graphics/Effects/SphereMapEffect.h>
#include <GTL/Graphics/Effects/SpotLightEffect.h>
#include <GTL/Graphics/Effects/TextEffect.h>
#include <GTL/Graphics/Effects/Texture2Effect.h>
#include <GTL/Graphics/Effects/Texture3Effect.h>
#include <GTL/Graphics/Effects/VertexColorEffect.h>
#include <GTL/Graphics/Effects/VisualEffect.h>
#include <GTL/Graphics/Effects/VolumeFogEffect.h>

// Resources
#include <GTL/Graphics/Resources/DataFormat.h>
#include <GTL/Graphics/Resources/Resource.h>

// Resources/Buffers
#include <GTL/Graphics/Resources/Buffers/Buffer.h>
#include <GTL/Graphics/Resources/Buffers/ConstantBuffer.h>
#include <GTL/Graphics/Resources/Buffers/IndexBuffer.h>
#include <GTL/Graphics/Resources/Buffers/IndexFormat.h>
#include <GTL/Graphics/Resources/Buffers/IndirectArgumentsBuffer.h>
#include <GTL/Graphics/Resources/Buffers/MemberLayout.h>
#include <GTL/Graphics/Resources/Buffers/RawBuffer.h>
#include <GTL/Graphics/Resources/Buffers/StructuredBuffer.h>
#include <GTL/Graphics/Resources/Buffers/TextureBuffer.h>
#include <GTL/Graphics/Resources/Buffers/TypedBuffer.h>
#include <GTL/Graphics/Resources/Buffers/VertexBuffer.h>
#include <GTL/Graphics/Resources/Buffers/VertexFormat.h>

// Resources/Textures
#include <GTL/Graphics/Resources/Textures/DrawTarget.h>
#include <GTL/Graphics/Resources/Textures/Texture.h>
#include <GTL/Graphics/Resources/Textures/Texture1.h>
#include <GTL/Graphics/Resources/Textures/Texture1Array.h>
#include <GTL/Graphics/Resources/Textures/Texture2.h>
#include <GTL/Graphics/Resources/Textures/Texture2Array.h>
#include <GTL/Graphics/Resources/Textures/Texture3.h>
#include <GTL/Graphics/Resources/Textures/TextureArray.h>
#include <GTL/Graphics/Resources/Textures/TextureCube.h>
#include <GTL/Graphics/Resources/Textures/TextureCubeArray.h>
#include <GTL/Graphics/Resources/Textures/TextureDS.h>
#include <GTL/Graphics/Resources/Textures/TextureRT.h>
#include <GTL/Graphics/Resources/Textures/TextureSingle.h>

// SceneGraph
#include <GTL/Graphics/SceneGraph/MeshFactory.h>

// SceneGraph/CollisionDetection
#include <GTL/Graphics/SceneGraph/CollisionDetection/CollisionGroup.h>
#include <GTL/Graphics/SceneGraph/CollisionDetection/CollisionRecord.h>
#include <GTL/Graphics/SceneGraph/CollisionDetection/CollisionMesh.h>

// SceneGraph/Controllers
#include <GTL/Graphics/SceneGraph/Controllers/BlendTransformController.h>
#include <GTL/Graphics/SceneGraph/Controllers/Controller.h>
#include <GTL/Graphics/SceneGraph/Controllers/ControlledObject.h>
#include <GTL/Graphics/SceneGraph/Controllers/IKController.h>
#include <GTL/Graphics/SceneGraph/Controllers/KeyframeController.h>
#include <GTL/Graphics/SceneGraph/Controllers/MorphController.h>
#include <GTL/Graphics/SceneGraph/Controllers/ParticleController.h>
#include <GTL/Graphics/SceneGraph/Controllers/PointController.h>
#include <GTL/Graphics/SceneGraph/Controllers/SkinController.h>
#include <GTL/Graphics/SceneGraph/Controllers/TransformController.h>

// SceneGraph/Detail
#include <GTL/Graphics/SceneGraph/Detail/BillboardNode.h>
#include <GTL/Graphics/SceneGraph/Detail/CLODCollapseRecord.h>
#include <GTL/Graphics/SceneGraph/Detail/CLODMesh.h>
#include <GTL/Graphics/SceneGraph/Detail/CLODMeshCreator.h>
#include <GTL/Graphics/SceneGraph/Detail/DLODNode.h>
#include <GTL/Graphics/SceneGraph/Detail/SwitchNode.h>

// SceneGraph/Hierarchy
#include <GTL/Graphics/SceneGraph/Hierarchy/BoundingSphere.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/Camera.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/Light.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/Node.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/Particles.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/PVWUpdater.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/Spatial.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/ViewVolume.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/ViewVolumeNode.h>
#include <GTL/Graphics/SceneGraph/Hierarchy/Visual.h>

// SceneGraph/Picking
#include <GTL/Graphics/SceneGraph/Picking/Picker.h>
#include <GTL/Graphics/SceneGraph/Picking/PickRecord.h>

// SceneGraph/Sorting
#include <GTL/Graphics/SceneGraph/Sorting/BspNode.h>

// SceneGraph/Terrain
#include <GTL/Graphics/SceneGraph/Terrain/Terrain.h>

// SceneGraph/Visibility
#include <GTL/Graphics/SceneGraph/Visibility/CullingPlane.h>
#include <GTL/Graphics/SceneGraph/Visibility/Culler.h>

// Shaders
#include <GTL/Graphics/Shaders/ComputeProgram.h>
#include <GTL/Graphics/Shaders/ProgramDefines.h>
#include <GTL/Graphics/Shaders/ProgramFactory.h>
#include <GTL/Graphics/Shaders/Shader.h>
#include <GTL/Graphics/Shaders/VisualProgram.h>

// State
#include <GTL/Graphics/State/BlendState.h>
#include <GTL/Graphics/State/DepthStencilState.h>
#include <GTL/Graphics/State/DrawingState.h>
#include <GTL/Graphics/State/RasterizerState.h>
#include <GTL/Graphics/State/SamplerState.h>
