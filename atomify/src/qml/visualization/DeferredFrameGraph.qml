import QtQuick 2.5
import QtQuick.Controls 1.4
import Atomify 1.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtQuick.Scene3D 2.0

import SimVis 1.0
import ShaderNodes 1.0

import "../desktop" // TODO should be separate controllers for desktop and mobile

Viewport {
    id: root

    property alias clearColor: clearBuffer.clearColor
    property real width: 10
    property real height: 10
    property alias blurTexture: blurTexture
    property alias normalTexture: normalTexture
    property alias colorTexture: colorTexture
    property alias depthTexture: depthTexture
    property alias ssaoTexture: ssaoTexture
    property alias positionTexture: positionTexture
    property alias surface: surfaceSelector.surface
    property Camera camera: Camera {}
    property Layer atomLayer
    property Layer guideLayer
    property Layer outlineLayer

    normalizedRect: Qt.rect(0.0, 0.0, 1.0, 1.0)

    TechniqueFilter {
        matchAll: FilterKey { name: "renderingStyle"; value: "deferred" }
        RenderSurfaceSelector {
            id: surfaceSelector
            RenderPassFilter {
                id : geometryPass
                matchAny : FilterKey { name : "pass"; value : "geometry" }
                RenderTargetSelector {
                    target: RenderTarget {
                        attachments : [
                            RenderTargetOutput {
                                objectName : "normalOut"
                                attachmentPoint : RenderTargetOutput.Color0
                                texture : Texture2D {
                                    id : normalTexture
                                    width : root.width
                                    height : root.height
                                    format : Texture.RGBA32F
                                    generateMipMaps : false
                                    magnificationFilter : Texture.Nearest
                                    minificationFilter : Texture.Nearest
                                    wrapMode {
                                        x: WrapMode.ClampToEdge
                                        y: WrapMode.ClampToEdge
                                    }
                                }
                            },
                            RenderTargetOutput {
                                objectName : "positionOut"
                                attachmentPoint : RenderTargetOutput.Color1
                                texture : Texture2D {
                                    id : positionTexture
                                    width : root.width
                                    height : root.height
                                    format : Texture.RGBA32F
                                    generateMipMaps : false
                                    magnificationFilter : Texture.Nearest
                                    minificationFilter : Texture.Nearest
                                    wrapMode {
                                        x: WrapMode.ClampToEdge
                                        y: WrapMode.ClampToEdge
                                    }
                                }
                            },
                            RenderTargetOutput {
                                objectName : "colorOut"
                                attachmentPoint : RenderTargetOutput.Color2
                                texture : Texture2D {
                                    id : colorTexture
                                    width : root.width
                                    height : root.height
                                    format : Texture.RGBA32F
                                    generateMipMaps : false
                                    magnificationFilter : Texture.Nearest
                                    minificationFilter : Texture.Nearest
                                    wrapMode {
                                        x: WrapMode.ClampToEdge
                                        y: WrapMode.ClampToEdge
                                    }
                                }
                            },
                            RenderTargetOutput {
                                objectName: "depth"
                                attachmentPoint: RenderTargetOutput.Depth
                                texture: Texture2D {
                                    id: depthTexture
                                    width : root.width
                                    height : root.height
                                    format: Texture.D32
                                    generateMipMaps: false
                                    magnificationFilter: Texture.Nearest
                                    minificationFilter: Texture.Nearest
                                    wrapMode {
                                        x: WrapMode.ClampToEdge
                                        y: WrapMode.ClampToEdge
                                    }
                                    comparisonFunction: Texture.CompareLessEqual
                                    comparisonMode: Texture.CompareNone
                                }
                            }
                        ]
                    }
                    CameraSelector {
                        camera: root.camera
                        LayerFilter {
                            layers: atomLayer
                            ClearBuffers {
                                id: clearBuffer
                                clearColor: "#000"
                                buffers: ClearBuffers.ColorDepthBuffer
                            }
                        }
                        LayerFilter {
                            layers: guideLayer
                        }
                        LayerFilter {
                            layers: outlineLayer
                        }
                    }
                }
            }
            RenderPassFilter {
                matchAny : FilterKey { name : "pass"; value : "ssao" }
                RenderTargetSelector {
                    target: RenderTarget {
                        attachments : [
                            RenderTargetOutput {
                                objectName : "ssao"
                                attachmentPoint : RenderTargetOutput.Color0
                                texture : Texture2D {
                                    id : ssaoTexture
                                    width : root.width
                                    height : root.height
                                    format : Texture.RGBA32F
                                    generateMipMaps : false
                                    magnificationFilter : Texture.Nearest
                                    minificationFilter : Texture.Nearest
                                    wrapMode {
                                        x: WrapMode.ClampToEdge
                                        y: WrapMode.ClampToEdge
                                    }
                                }
                            }
                        ]
                    }
                    ClearBuffers {
                        clearColor: "#000"
                        buffers: ClearBuffers.ColorDepthBuffer
                        CameraSelector {
                            camera: root.camera
                        }
                    }
                }
            }
            RenderPassFilter {
                matchAny : FilterKey { name : "pass"; value : "blur" }
                RenderTargetSelector {
                    target: RenderTarget {
                        attachments : [
                            RenderTargetOutput {
                                objectName : "blur"
                                attachmentPoint : RenderTargetOutput.Color0
                                texture : Texture2D {
                                    id : blurTexture
                                    width : root.width
                                    height : root.height
                                    format : Texture.RGBA32F
                                    generateMipMaps : false
                                    magnificationFilter : Texture.Nearest
                                    minificationFilter : Texture.Nearest
                                    wrapMode {
                                        x: WrapMode.ClampToEdge
                                        y: WrapMode.ClampToEdge
                                    }
                                }
                            }
                        ]
                    }
                    ClearBuffers {
                        clearColor: "#000"
                        buffers: ClearBuffers.ColorDepthBuffer
                        CameraSelector {
                            camera: root.camera
                        }
                    }
                }
            }
            RenderPassFilter {
                matchAny : FilterKey { name : "pass"; value : "final" }
                ClearBuffers {
                    clearColor: "#000"
                    buffers: ClearBuffers.ColorDepthBuffer
                    CameraSelector {
                        id: viewCameraSelector
                        camera: root.camera
                    }
                }
            }
        }
    }
}
