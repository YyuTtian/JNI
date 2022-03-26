#include <jni.h>
#include <string>
#include "fmod.hpp"
#include <android/log.h>
#include <unistd.h>

#define TAG "qglogImpl" // 这个是自定义的LOG的TAG
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型


const char *name = "广雷";

void func() {
    LOGI("%s自己实现的函数", name);
}

using namespace FMOD;
#define normal 0
#define luoli 1
#define dashu 2
#define gaoguai 3
#define jingsong 4
#define kongling 5

const char *changeVoice(const char *path, int mode) {

    LOGI("path=%s", path);

    const char *content_ = "默认 播放完毕";


    System *system = 0;
    Sound *sound = 0;
    Channel *channel = 0;
    DSP *dsp = 0;

    // 初始化
    System_Create(&system);

    // 最大音轨数
    // 系统初始化标记
    // 额外数据
    system->init(32, FMOD_INIT_NORMAL, 0);

    // 路径
    // 声音初始化标记
    // 额外数据
    // 声音指针
    system->createSound(path, FMOD_DEFAULT, 0, &sound);

    // 声音
    // 分组音轨
    // 控制
    // 通道
    system->playSound(sound, 0, false, &channel);

    switch (mode) {
        case normal: // 原生
            content_ = "原生 播放完毕";
            break;
        case luoli: // 萝莉
            content_ = "萝莉 播放完毕";

            // 音调高 -- 萝莉 2.0
            // 1.创建DSP类型的Pitch 音调条件
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            // 2.设置Pitch音调调节2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 2.0f);
            // 3.添加音效进去 音轨
            channel->addDSP(0, dsp);
            break;
        case dashu: // 大叔
            content_ = "大叔 播放完毕";

            // 音调低 -- 大叔 0.7
            // 1.创建DSP类型的Pitch 音调条件
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            // 2.设置Pitch音调调节2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7f);
            // 3.添加音效进去 音轨
            channel->addDSP(0, dsp);
            break;
        case gaoguai: // 搞怪
            content_ = "搞怪小黄人 播放完毕";

            // 小黄人声音 频率快

            // 从音轨拿 当前 频率
            float mFrequency;
            channel->getFrequency(&mFrequency);

            // 修改频率
            channel->setFrequency(mFrequency * 1.5f); // 频率加快  小黄人的声音
            break;
        case jingsong: // 惊悚
            content_ = "惊悚 播放完毕";

            // 惊悚音效：特点： 很多声音的拼接

            // TODO 音调低
            // 音调低 -- 大叔 0.7
            // 1.创建DSP类型的Pitch 音调条件
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            // 2.设置Pitch音调调节2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7f);
            // 3.添加音效进去 音轨
            channel->addDSP(0, dsp); // 第一个音轨

            // TODO 搞点回声
            // 回音 ECHO
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200); // 回音 延时    to 5000.  Default = 500.
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 10); // 回音 衰减度 Default = 50   0 完全衰减了
            channel->addDSP(1, dsp); // 第二个音轨

            // TODO 颤抖
            // Tremolo 颤抖音 正常5    非常颤抖  20
            system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_FREQUENCY, 20); // 非常颤抖
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 0.8f); // ？？？
            channel->addDSP(2, dsp); // 第三个音轨

            // 调音师：才能跳出来  同学们自己去调
            break;
        case kongling: // 空灵  学校广播
            content_ = "空灵 播放完毕";

            // 回音 ECHO
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200); // 回音 延时    to 5000.  Default = 500.
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 10); // 回音 衰减度 Default = 50   0 完全衰减了
            channel->addDSP(0, dsp);
            break;

    }

    // 等待播放完毕 再回收
    bool isPlayer = true; // 你用不是一级指针  我用一级指针接收你，可以修改给你
    while (isPlayer) {
        LOGI("在等待");
        channel->isPlaying(&isPlayer); // 如果真的播放完成了，音轨是知道的，内部会修改isPlayer=false
        usleep(1000 * 1000); // 每个一秒
    }

    // 时时刻刻记得回收
    sound->release();
    system->close();
    system->release();

    return content_;
}