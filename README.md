# 浏览器指纹

随机 [Chromium](https://github.com/yanminhui/chromium) 浏览器指纹。

- [ ] Cookie
- [ ] User-Agent
- [ ] 时区
- [ ] WebRTC
- [ ] 地理位置
- [ ] 语言
- [ ] 屏幕分辨率
- [ ] 字体
- [ ] Canvas 图像噪音
- [x] WebGL 图像噪音
- [x] WebGL 元数据掩蔽 
- [ ] AudioContext
- [ ] 媒体设备
- [ ] ClientRects
- [ ] SpeechVoices
- [ ] 硬件并发数
- [ ] 设备内存
- [ ] 设备名称
- [ ] Mac 地址
- [ ] Do Not Track
- [ ] Flask
- [ ] 端口扫描保护
- [ ] 硬件加速

## WebGL 图像噪音

```cpp
void WebGLRenderingContextBase::BufferDataImpl(GLenum target,
                                               int64_t size,
                                               const void* data,
                                               GLenum usage) {

  // ...
  buffer->SetSize(size);

  if (data && 0 < size && size % sizeof(GLfloat) == 0) {
    // 生成一个随机噪音序列，而不是一个随机数避免不够随机。
    WTF::Vector<GLfloat> noises;
    fingerprint::GLnewNoiseFloatArray(noises);

    // 构建一个缓存对象的副本，并应用噪音序列。
    auto len = size / sizeof(GLfloat);
    GLfloat out[len];
    fingerprint::GLapplyNoise(out, data, len, noises);

    ContextGL()->BufferData(target, static_cast<GLsizeiptr>(size), out, usage);
    return ;
  }

  ContextGL()->BufferData(target, static_cast<GLsizeiptr>(size), data, usage);
}
```

## WebGL 元数据掩蔽

```cpp
ScriptValue WebGLRenderingContextBase::getParameter(ScriptState* script_state,
                                                    GLenum pname) {
    // ...
    case WebGLDebugRendererInfo::kUnmaskedRendererWebgl:
      if (ExtensionEnabled(kWebGLDebugRendererInfoName)) {
        // 在实际应用中应考虑基于 vendor 来处理 renderer。
        fingerprint::GLrendererPool rendererp;
        const std::string vendor = rendererp.NewVendor();
        const std::string renderer = rendererp.NewRenderer(vendor);
        if (IdentifiabilityStudySettings::Get()->ShouldSampleType(
                blink::IdentifiableSurface::Type::kWebGLParameter)) {
          RecordIdentifiableGLParameterDigest(
              pname, IdentifiabilityBenignStringToken(
                         String(/*ContextGL()->GetString(GL_RENDERER)*/renderer)));
        }
        return WebGLAny(script_state,
                        String(/*ContextGL()->GetString(GL_RENDERER)*/renderer));
      }
      // ...
    case WebGLDebugRendererInfo::kUnmaskedVendorWebgl:
      if (ExtensionEnabled(kWebGLDebugRendererInfoName)) {
        const std::string vendor = fingerprint::GLrendererPool{}.NewVendor();
        if (IdentifiabilityStudySettings::Get()->ShouldSampleType(
                blink::IdentifiableSurface::Type::kWebGLParameter)) {
          RecordIdentifiableGLParameterDigest(
              pname, IdentifiabilityBenignStringToken(
                         String(/*ContextGL()->GetString(GL_VENDOR)*/vendor)));
        }
        return WebGLAny(script_state,
                        String(/*ContextGL()->GetString(GL_VENDOR)*/vendor));
      }
    // ...
}
```