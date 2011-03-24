#pragma once
/*
 *      Copyright (C) 2005-2011 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "addons/Addon.h"
#include "addons/AddonDll.h"
#include "addons/DllPVRClient.h"
#include "pvr/epg/PVREpgContainer.h"
#include "pvr/channels/PVRChannelGroup.h"
#include "pvr/timers/PVRTimers.h"
#include "pvr/timers/PVRTimerInfoTag.h"
#include "pvr/recordings/PVRRecordings.h"

typedef std::vector<PVR_MENUHOOK> PVR_MENUHOOKS;

class IPVRClientCallback
{
public:
  virtual void OnClientMessage(const int clientID, const PVR_EVENT clientEvent, const char* msg)=0;
};

/*!
 * Interface from XBMC to a PVR add-on.
 *
 * Also translates XBMC's C++ structures to the addon's C structures.
 */
class CPVRClient : public ADDON::CAddonDll<DllPVRClient, PVRClient, PVR_PROPS>
{
public:
  CPVRClient(const ADDON::AddonProps& props);
  CPVRClient(const cp_extension_t *ext);
  ~CPVRClient(void);

  /*!
   * @brief Initialise the instance of this add-on.
   * @param iClientId The ID of this add-on.
   * @param pvrCB The call-back to use.
   * @return True if it was created successfully, false otherwise.
   */
  bool Create(int iClientId, IPVRClientCallback *pvrCB);

  /*!
   * @brief Destroy the instance of this add-on.
   */
  void Destroy(void);

  /*!
   * @brief Destroy and recreate this add-on.
   * @return True if the add-on was reinitialised successfully.
   */
  bool ReCreate(void);

  /*!
   * @return True if this instance is initialised, false otherwise.
   */
  bool ReadyToUse(void) const;

  /*!
   * @return The ID of this instance.
   */
  int GetID(void) const;

  /*!
   * @brief Change a setting in the add-on.
   * @param settingName The name of the setting.
   * @param settingValue The new value.
   * @return The status reported by the add-on.
   */
  virtual ADDON_STATUS SetSetting(const char *settingName, const void *settingValue);

  /*!
   * @brief Query this add-on's capabilities.
   * @param props The add-on properties.
   * @return PVR_ERROR_NO_ERROR if the properties were fetched successfully.
   */
  PVR_ERROR GetProperties(PVR_SERVERPROPS *props);

  /*!
   * @return The name reported by the backend.
   */
  const char *GetBackendName(void);

  /*!
   * @return The version string reported by the backend.
   */
  const char *GetBackendVersion(void);

  /*!
   * @return The connection string reported by the backend.
   */
  const char *GetConnectionString(void);

  /*!
   * @return A friendly name for this add-on that can be used in log messages.
   */
  const char *GetFriendlyName(void);

  /*!
   * @brief Get the disk space reported by the server.
   * @param iTotal The total disk space.
   * @param iUsed The used disk space.
   * @return PVR_ERROR_NO_ERROR if the drive space has been fetched successfully.
   */
  PVR_ERROR GetDriveSpace(long long *iTotal, long long *iUsed);

  /*!
   * @brief Get the time reported by the backend.
   * @param localTime The local time.
   * @param iGmtOffset The GMT offset used.
   * @return PVR_ERROR_NO_ERROR if the time has been fetched successfully.
   */
  PVR_ERROR GetBackendTime(time_t *localTime, int *iGmtOffset);

  /*!
   * @brief Start a channel scan on the server.
   * @return PVR_ERROR_NO_ERROR if the channel scan has been started successfully.
   */
  PVR_ERROR StartChannelScan(void);

  /*!
   * @brief Get the correction to use to convert local time.
   * @return The correction in seconds.
   */
  int GetTimeCorrection(void);

  /*!
   * @return The ID of the client.
   */
  int GetClientID(void) const;

  /*!
   * @return True if this add-on has menu hooks, false otherwise.
   */
  bool HaveMenuHooks(void) const;

  /*!
   * @return The menu hooks for this add-on.
   */
  PVR_MENUHOOKS *GetMenuHooks(void);

  /*!
   * @brief Call one of the menu hooks of this client.
   * @param hook The hook to call.
   */
  void CallMenuHook(const PVR_MENUHOOK &hook);

  /*!
   * @brief Request an EPG table for a channel from the client.
   * @param channel The channel to get the EPG table for.
   * @param epg The table to write the data to.
   * @param start The start time to use.
   * @param end The end time to use.
   * @param bSaveInDb If true, tell the callback method to save any new entry in the database or not. see CAddonCallbacksPVR::PVRTransferEpgEntry()
   * @return PVR_ERROR_NO_ERROR if the table has been fetched successfully.
   */
  PVR_ERROR GetEPGForChannel(const CPVRChannel &channel, CPVREpg *epg, time_t start = 0, time_t end = 0, bool bSaveInDb = false);

  /*!
   * @return The total amount of channels on the server or -1 on error.
   */
  int GetNumChannels(void);

  /*!
   * @brief Request the list of all channels from the backend.
   * @param channels The channel group to add the channels to.
   * @param bRadio True to get the radio channels, false to get the TV channels.
   * @return PVR_ERROR_NO_ERROR if the list has been fetched successfully.
   */
  PVR_ERROR GetChannelList(CPVRChannelGroup &channels, bool bRadio);

  /*!
   * @return The total amount of channels on the server or -1 on error.
   */
  int GetNumRecordings(void);

  /*!
   * @brief Request the list of all recordings from the backend.
   * @param results The container to add the recordings to.
   * @return PVR_ERROR_NO_ERROR if the list has been fetched successfully.
   */
  PVR_ERROR GetAllRecordings(CPVRRecordings *results);

  /*!
   * @brief Delete a recording on the backend.
   * @param recording The recording to delete.
   * @return PVR_ERROR_NO_ERROR if the recording has been deleted successfully.
   */
  PVR_ERROR DeleteRecording(const CPVRRecording &recording);

  /*!
   * @brief Rename a recording on the backend.
   * @param recording The recording to rename.
   * @param strNewName The new name.
   * @return PVR_ERROR_NO_ERROR if the recording has been renamed successfully.
   */
  PVR_ERROR RenameRecording(const CPVRRecording &recording, const CStdString &strNewName);

  /*!
   * @return The total amount of timers on the backend or -1 on error.
   */
  int GetNumTimers(void);

  /*!
   * @brief Request the list of all timers from the backend.
   * @param results The container to store the result in.
   * @return PVR_ERROR_NO_ERROR if the list has been fetched successfully.
   */
  PVR_ERROR GetAllTimers(CPVRTimers *results);

  /*!
   * @brief Add a timer on the backend.
   * @param timer The timer to add.
   * @return PVR_ERROR_NO_ERROR if the timer has been added successfully.
   */
  PVR_ERROR AddTimer(const CPVRTimerInfoTag &timer);

  /*!
   * @brief Delete a timer on the backend.
   * @param timer The timer to delete.
   * @param bForce Set to true to delete a timer that is currently recording a program.
   * @return PVR_ERROR_NO_ERROR if the timer has been deleted successfully.
   */
  PVR_ERROR DeleteTimer(const CPVRTimerInfoTag &timer, bool bForce = false);

  /*!
   * @brief Rename a timer on the server.
   * @param timer The timer to rename.
   * @param strNewName The new name of the timer.
   * @return PVR_ERROR_NO_ERROR if the timer has been renamed successfully.
   */
  PVR_ERROR RenameTimer(const CPVRTimerInfoTag &timer, const CStdString &strNewName);

  /*!
   * @brief Update the timer information on the server.
   * @param timer The timer to update.
   * @return PVR_ERROR_NO_ERROR if the timer has been updated successfully.
   */
  PVR_ERROR UpdateTimer(const CPVRTimerInfoTag &timer);

  /*!
   * @brief Open a live stream on the server.
   * @param channel The channel to stream.
   * @return True if the stream opened successfully, false otherwise.
   */
  bool OpenLiveStream(const CPVRChannel &channel);

  /*!
   * @brief Close an open live stream.
   */
  void CloseLiveStream(void);

  /*!
   * @brief Read from an open live stream.
   * @param lpBuf The buffer to store the data in.
   * @param uiBufSize The amount of bytes to read.
   * @return The amount of bytes that were actually read from the stream.
   */
  int ReadLiveStream(void* lpBuf, int64_t uiBufSize);

  /*!
   * @brief Seek in a live stream on a backend that supports timeshifting.
   * @param iFilePosition The position to seek to.
   * @param iWhence ?
   * @return The new position.
   */
  int64_t SeekLiveStream(int64_t iFilePosition, int iWhence = SEEK_SET);

  /*!
   * @return The position in the stream that's currently being read.
   */
  int64_t PositionLiveStream(void);

  /*!
   * @return The total length of the stream that's currently being read.
   */
  int64_t LengthLiveStream(void);

  /*!
   * @return The channel number on the server of the live stream that's currently being read.
   */
  int GetCurrentClientChannel(void);

  /*!
   * @brief Switch to another channel. Only to be called when a live stream has already been opened.
   * @param channel The channel to switch to.
   * @return True if the switch was successful, false otherwise.
   */
  bool SwitchChannel(const CPVRChannel &channel);

  /*!
   * @brief Get the signal quality of the stream that's currently open.
   * @param qualityinfo The signal quality.
   * @return True if the signal quality has been read successfully, false otherwise.
   */
  bool SignalQuality(PVR_SIGNALQUALITY &qualityinfo);

  /*!
   * @brief Get the stream URL for a channel from the server. Used by the MediaPortal add-on.
   * @param channel The channel to get the stream URL for.
   * @return The requested URL.
   */
  const char *GetLiveStreamURL(const CPVRChannel &channel);

  /*!
   * @brief Open a recording on the server.
   * @param recording The recording to open.
   * @return True if the stream has been opened succesfully, false otherwise.
   */
  bool OpenRecordedStream(const CPVRRecording &recording);

  /*!
   * @brief Close an open stream from a recording.
   */
  void CloseRecordedStream(void);

  /*!
   * @brief Read from a recording.
   * @param lpBuf The buffer to store the data in.
   * @param uiBufSize The amount of bytes to read.
   * @return The amount of bytes that were actually read from the stream.
   */
  int ReadRecordedStream(void* lpBuf, int64_t uiBufSize);

  /*!
   * @brief Seek in a recorded stream.
   * @param iFilePosition The position to seek to.
   * @param iWhence ?
   * @return The new position.
   */
  int64_t SeekRecordedStream(int64_t iFilePosition, int iWhence = SEEK_SET);

  /*!
   * @return The position in the stream that's currently being read.
   */
  int64_t PositionRecordedStream(void);

  /*!
   * @return The total length of the stream that's currently being read.
   */
  int64_t LengthRecordedStream(void);

  /*!
   * @brief Get the stream properties of the stream that's currently being read.
   * @param props The properties.
   * @return PVR_ERROR_NO_ERROR if the properties have been fetched successfully.
   */
  PVR_ERROR GetStreamProperties(PVR_STREAMPROPS *props);

  /*!
   * @brief Reset the demultiplexer in the add-on.
   */
  void DemuxReset(void);

  /*!
   * @brief Abort the demultiplexer thread in the add-on.
   */
  void DemuxAbort(void);

  /*!
   * @brief Flush all data that's currently in the demultiplexer buffer in the add-on.
   */
  void DemuxFlush(void);

  /*!
   * @brief Read a packet from the demultiplexer.
   * @return The packet.
   */
  DemuxPacket *DemuxRead(void);

protected:
  bool                  m_bReadyToUse;          /*!< true if this add-on is connected to the backend, false otherwise */
  IPVRClientCallback   *m_manager;              /*!< the callback to use for this add-on */
  CStdString            m_strHostName;          /*!< the host name */
  CCriticalSection      m_critSection;          /*!< mutex for this class */
  PVR_MENUHOOKS         m_menuhooks;            /*!< the menu hooks for this add-on */

  /* cached data */
  int                   m_iTimeCorrection;      /*!< the cached time correction value */
  bool                  m_bGotTimeCorrection;   /*!< true if the time correction value has already been fetched */
  CStdString            m_strBackendName;       /*!< the cached backend version */
  bool                  m_bGotBackendName;      /*!< true if the backend name has already been fetched */
  CStdString            m_strBackendVersion;    /*!< the cached backend version */
  bool                  m_bGotBackendVersion;   /*!< true if the backend version has already been fetched */
  CStdString            m_strConnectionString;  /*!< the cached connection string */
  bool                  m_bGotConnectionString; /*!< true if the connection string has already been fetched */
  CStdString            m_strFriendlyName;      /*!< the cached friendly name */
  bool                  m_bGotFriendlyName;     /*!< true if the friendly name has already been fetched */
  PVR_SERVERPROPS       m_serverProperties;     /*!< the cached server properties */
  bool                  m_bGotServerProperties; /*!< true if the server properties have already been fetched */

private:
  /*!
   * @brief Copy over channel info from xbmcChannel to addonClient.
   * @param xbmcChannel The channel on XBMC's side.
   * @param addonChannel The channel on the addon's side.
   */
  void WriteClientChannelInfo(const CPVRChannel &xbmcChannel, PVR_CHANNEL &addonChannel);

  /*!
   * @brief Copy over timer info from xbmcTimer to addonTimer.
   * @param xbmcTimer The timer on XBMC's side.
   * @param addonTimer The timer on the addon's side.
   */
  void WriteClientTimerInfo(const CPVRTimerInfoTag &xbmcTimer, PVR_TIMERINFO &addonTimer);

  /*!
   * @brief Copy over recording info from xbmcRecording to addonRecording.
   * @param xbmcRecording The recording on XBMC's side.
   * @param addonRecording The recording on the addon's side.
   */
  void WriteClientRecordingInfo(const CPVRRecording &xbmcRecording, PVR_RECORDINGINFO &addonRecording);

  /*!
   * @brief Get the time correction value from the server and store it locally.
   */
  void SetTimeCorrection(void);

  /*!
   * @brief Get the backend name from the server and store it locally.
   */
  void SetBackendName(void);

  /*!
   * @brief Get the backend version from the server and store it locally.
   */
  void SetBackendVersion(void);

  /*!
   * @brief Get the connection string from the server and store it locally.
   */
  void SetConnectionString(void);

  /*!
   * @brief Get the friendly from the server and store it locally.
   */
  void SetFriendlyName(void);

  /*!
   * @brief Get the backend properties from the server and store it locally.
   */
  PVR_ERROR SetProperties(void);
};
