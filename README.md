# 浏览器指纹

随机 Chromium 浏览器指纹。

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

**Chromium 开发分支**：https://github.com/yanminhui/chromium/tree/dev

## Canves 图像噪音

- 坐标偏移
- RGB 噪音
- 渐变比例噪音
- 渐变颜色噪音
- 文本测量噪音：控制在单字范围内偏差

## WebGL 图像噪音

```cpp
void WebGLRenderingContextBase::BufferDataImpl(GLenum target,
                                               int64_t size,
                                               const void* data,
                                               GLenum usage) {

  // ...
  buffer->SetSize(size);

  using namespace fingerprint;
  if (data && 0 < size && size % sizeof(GLfloat) == 0 &&
      FPcontextPtr()->GetSettings().HasKey(gkWebGLImageNoises)) {
    // 使用噪音序列，而不是单个噪音避免碰撞。
    using FloatArray = WTF::Vector<GLfloat>;
    static base::NoDestructor<FloatArray> pnoises{[]() {
      FloatArray fa;
      FPcontextPtr()->GetSettings().Get(gkWebGLImageNoises, fa);
      return fa;
    }()}; // 缓存数据，避免重复解析噪音序列。

    auto len = size / sizeof(GLfloat);
    GLfloat out[len];
    GLapplyNoise(out, data, len, *pnoises);
    ContextGL()->BufferData(target, static_cast<GLsizeiptr>(size), out, usage);
    return;
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
        using namespace fingerprint;
        auto renderer = String(ContextGL()->GetString(GL_RENDERER));
        // 当解析不到定制的配置时，使用实际值。
        if (FPcontextPtr()->GetSettings().HasKey(gkWebGLRenderer)) {
          renderer =
              String(FPcontextPtr()->GetSettings().GetString(gkWebGLRenderer));
        }
        if (IdentifiabilityStudySettings::Get()->ShouldSampleType(
                blink::IdentifiableSurface::Type::kWebGLParameter)) {
          RecordIdentifiableGLParameterDigest(
              pname, IdentifiabilityBenignStringToken(renderer));
        }
        return WebGLAny(script_state, renderer);
      }
      SynthesizeGLError(
          GL_INVALID_ENUM, "getParameter",
          "invalid parameter name, WEBGL_debug_renderer_info not enabled");
      return ScriptValue::CreateNull(script_state->GetIsolate());
    case WebGLDebugRendererInfo::kUnmaskedVendorWebgl:
      if (ExtensionEnabled(kWebGLDebugRendererInfoName)) {
        using namespace fingerprint;
        auto vendor = String(ContextGL()->GetString(GL_VENDOR));
        // 当解析不到定制的配置时，使用实际值。
        if (FPcontextPtr()->GetSettings().HasKey(gkWebGLVendor)) {
          vendor =
              String(FPcontextPtr()->GetSettings().GetString(gkWebGLVendor));
        }
        if (IdentifiabilityStudySettings::Get()->ShouldSampleType(
                blink::IdentifiableSurface::Type::kWebGLParameter)) {
          RecordIdentifiableGLParameterDigest(
              pname, IdentifiabilityBenignStringToken(vendor));
        }
        return WebGLAny(script_state, vendor);
      }
      SynthesizeGLError(
          GL_INVALID_ENUM, "getParameter",
          "invalid parameter name, WEBGL_debug_renderer_info not enabled");
      return ScriptValue::CreateNull(script_state->GetIsolate());   
  // ...
}
```
