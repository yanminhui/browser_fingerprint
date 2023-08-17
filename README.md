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
- [ ] WebGL 元数据掩蔽 
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