#include "stdafx.h"
#include "AudioProcAPi.h"

CAudioProc::CAudioProc(AudioApi emAudioApi)
{
	m_audioInterface = NULL;
}

CAudioProc::~CAudioProc()
{
}

void CAudioProc::initApi(AudioApi emAudioApi)
{
}

void CAudioProc::unInitApi()
{
	if (m_audioInterface != NULL)
	{
		delete m_audioInterface;
		m_audioInterface = NULL;
	}
}

void CAudioProc::getCurrentApi()
{
}

void CAudioProc::getEnumDevice()
{
}

bool CAudioProc::isStreamOpen()
{
	return true;
}

void CAudioProc::openStream()
{
}

void CAudioProc::closeStream()
{
}

void CAudioProc::startStream()
{
}

void CAudioProc::stopStream()
{
}

void CAudioProc::abortStream()
{
}