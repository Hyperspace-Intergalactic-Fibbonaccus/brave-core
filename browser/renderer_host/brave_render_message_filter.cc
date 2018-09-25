/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/renderer_host/brave_render_message_filter.h"

#include "brave/browser/brave_browser_process_impl.h"
#include "brave/components/brave_shields/browser/tracking_protection_service.h"
#include "brave/components/brave_shields/browser/brave_shields_util.h"
#include "brave/components/brave_shields/common/brave_shield_constants.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/content_settings/tab_specific_content_settings.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/render_messages.h"
#include "components/content_settings/core/browser/cookie_settings.h"
#include "content/public/browser/browser_thread.h"

using content::BrowserThread;

BraveRenderMessageFilter::BraveRenderMessageFilter(int render_process_id, 
	Profile* profile)
	: ChromeRenderMessageFilter(render_process_id, profile),
	  host_content_settings_map_(HostContentSettingsMapFactory::GetForProfile(profile)),
	  weak_factory_(this) {
}

BraveRenderMessageFilter::~BraveRenderMessageFilter() {}

bool BraveRenderMessageFilter::OnMessageReceived(const IPC::Message& message) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(BraveRenderMessageFilter, message)
    IPC_MESSAGE_HANDLER(ChromeViewHostMsg_AllowDatabase, OnAllowDatabase);
    IPC_MESSAGE_HANDLER(ChromeViewHostMsg_AllowDOMStorage, OnAllowDOMStorage);
    IPC_MESSAGE_HANDLER(ChromeViewHostMsg_AllowIndexedDB, OnAllowIndexedDB);
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  if (handled) return true; 
  return ChromeRenderMessageFilter::OnMessageReceived(message);
}

bool BraveRenderMessageFilter::ShouldStoreState(const GURL& origin_url, 
	const GURL& top_origin_url) {
	CHECK(g_brave_browser_process->tracking_protection_service()->IsInitialized());

	bool allow_brave_shields = brave_shields::IsAllowContentSetting(
      host_content_settings_map_, top_origin_url, top_origin_url, CONTENT_SETTINGS_TYPE_PLUGINS,
      brave_shields::kBraveShields);

	bool allow_trackers = brave_shields::IsAllowContentSetting(
      host_content_settings_map_, top_origin_url, top_origin_url, CONTENT_SETTINGS_TYPE_PLUGINS, 
      brave_shields::kTrackers);

  	return !(allow_brave_shields && !allow_trackers &&
      !g_brave_browser_process->tracking_protection_service()->ShouldStoreState(origin_url)) &&
  		cookie_settings_->IsCookieAccessAllowed(origin_url, top_origin_url);;
}

void BraveRenderMessageFilter::OnAllowDatabase(int render_frame_id,
               const GURL& origin_url,
               const GURL& top_origin_url,
               const base::string16& name,
               const base::string16& display_name,
               bool* allowed) {
	*allowed = ShouldStoreState(origin_url, top_origin_url);
      
  	BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(&TabSpecificContentSettings::WebDatabaseAccessed,
                 render_process_id_, render_frame_id, origin_url, name,
                 display_name, !*allowed));
}

void BraveRenderMessageFilter::OnAllowDOMStorage(int render_frame_id,
                                                  const GURL& origin_url,
                                                  const GURL& top_origin_url,
                                                  bool local,
                                                  bool* allowed) {
  *allowed = ShouldStoreState(origin_url, top_origin_url);
     
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(&TabSpecificContentSettings::DOMStorageAccessed,
                 render_process_id_, render_frame_id, origin_url, local,
                 !*allowed));
}

void BraveRenderMessageFilter::OnAllowIndexedDB(int render_frame_id,
                                                 const GURL& origin_url,
                                                 const GURL& top_origin_url,
                                                 const base::string16& name,
                                                 bool* allowed) {
  *allowed = ShouldStoreState(origin_url, top_origin_url);

  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(&TabSpecificContentSettings::IndexedDBAccessed,
                 render_process_id_, render_frame_id, origin_url, name,
                 !*allowed));
}