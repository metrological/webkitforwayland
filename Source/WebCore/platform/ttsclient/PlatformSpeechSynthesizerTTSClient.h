/* Copyright (C) 2019 RDK Management.  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS. OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef PlatformSpeechSynthesizerTTSClient_h
#define PlatformSpeechSynthesizerTTSClient_h

#if ENABLE(SPEECH_SYNTHESIS) && USE(TTS_CLIENT)

#include "PlatformSpeechSynthesizer.h"
#include "TTSClient.h"
#include <wtf/WeakPtr.h>
#include <wtf/Forward.h>

namespace WebCore {

class PlatformSpeechSynthesizerTTSClient : public PlatformSpeechSynthesizer
                                         , public TTS::TTSConnectionCallback
                                         , public TTS::TTSSessionCallback
                                         , public CanMakeWeakPtr<PlatformSpeechSynthesizerTTSClient> {
public:
    explicit PlatformSpeechSynthesizerTTSClient(PlatformSpeechSynthesizerClient*);
    void setPageMediaVolume(float volume);

    // PlatformSpeechSynthesizer
    virtual ~PlatformSpeechSynthesizerTTSClient() override;
    virtual const Vector<RefPtr<PlatformSpeechSynthesisVoice>>& voiceList() const override;
    virtual void initializeVoiceList() override;
    virtual void speak(RefPtr<PlatformSpeechSynthesisUtterance>&&) override;
    virtual void pause() override;
    virtual void resume() override;
    virtual void cancel() override;

    // TTSConnectionCallback
    virtual void onTTSServerConnected() override;
    virtual void onTTSServerClosed() override;
    virtual void onTTSStateChanged(bool enabled) override;
    virtual void onVoiceChanged(std::string voice) override;

    // TTSSessionCallback
    virtual void onTTSSessionCreated(uint32_t appId, uint32_t sessionId) override;
    virtual void onResourceAcquired(uint32_t appId, uint32_t sessionId) override;
    virtual void onResourceReleased(uint32_t appId, uint32_t sessionId) override;
    virtual void onWillSpeak(uint32_t appId, uint32_t sessionId, TTS::SpeechData &data) override;
    virtual void onSpeechStart(uint32_t appId, uint32_t sessionId, TTS::SpeechData &data) override;
    virtual void onSpeechPause(uint32_t appId, uint32_t sessionId, uint32_t speechId) override;
    virtual void onSpeechResume(uint32_t appId, uint32_t sessionId, uint32_t speechId) override;
    virtual void onSpeechCancelled(uint32_t appId, uint32_t sessionId, uint32_t speechId) override;
    virtual void onSpeechInterrupted(uint32_t appId, uint32_t sessionId, uint32_t speechId) override;
    virtual void onNetworkError(uint32_t appId, uint32_t sessionId, uint32_t speechId) override;
    virtual void onPlaybackError(uint32_t appId, uint32_t sessionId, uint32_t speechId) override;
    virtual void onSpeechComplete(uint32_t appId, uint32_t sessionId, TTS::SpeechData &data) override;

private:
    void speakingFinished(uint32_t speechId, std::optional<SpeechSynthesisErrorCode> error);
    void notifyClient(RefPtr<PlatformSpeechSynthesisUtterance>, std::optional<SpeechSynthesisErrorCode> error);

    RefPtr<PlatformSpeechSynthesisUtterance> m_firstUtterance;
    RefPtr<PlatformSpeechSynthesisUtterance> m_currentUtterance;
    HashMap<uint32_t, RefPtr<PlatformSpeechSynthesisUtterance>> m_utterancesInProgress;
    WeakPtrFactory<PlatformSpeechSynthesizerTTSClient> m_weakPtrFactory;
    bool m_shouldCacheUtterance;

    // TTS
    TTS::TTSClient *m_ttsClient;
    uint32_t m_ttsSessionId;
    uint32_t m_appId;
    bool m_ttsEnabled;
    bool m_ttsConnected;
    uint32_t m_currentUtteranceSpeechId;

    static double m_TTSVolume;
    static double m_TTSRate;
};

} // namespace WebCore

#endif // ENABLE(SPEECH_SYNTHESIS) && USE(TTS_CLIENT)

#endif // PlatformSpeechSynthesizer_h
