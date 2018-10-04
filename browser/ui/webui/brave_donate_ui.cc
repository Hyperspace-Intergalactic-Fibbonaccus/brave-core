/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/webui/brave_donate_ui.h"

#include "brave/browser/brave_browser_process_impl.h"
#include "brave/browser/ui/webui/basic_ui.h"
#include "brave/common/pref_names.h"
#include "brave/common/webui_url_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/chrome_pages.h"
#include "chrome/browser/ui/webui/constrained_web_dialog_ui.h"
#include "chrome/common/pref_names.h"
#include "chrome/common/webui_url_constants.h"
#include "components/grit/brave_components_resources.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "brave/components/brave_rewards/browser/rewards_service.h"
#include "brave/components/brave_rewards/browser/rewards_service_factory.h"
#include "brave/components/brave_rewards/browser/rewards_service_observer.h"

using content::WebUIMessageHandler;

namespace {

// The handler for Javascript messages for Brave about: pages
class RewardsDonateDOMHandler : public WebUIMessageHandler,
                                public brave_rewards::RewardsServiceObserver {
public:
  RewardsDonateDOMHandler() {};
  ~RewardsDonateDOMHandler() override;

  void Init();

  // WebUIMessageHandler implementation.
  void RegisterMessages() override;

private:
  void GetPublisherDonateData(const base::ListValue* args);

  // RewardsServiceObserver implementation

  brave_rewards::RewardsService* rewards_service_;  // NOT OWNED

  DISALLOW_COPY_AND_ASSIGN(RewardsDonateDOMHandler);
};

RewardsDonateDOMHandler::~RewardsDonateDOMHandler() {
  if (rewards_service_)
    rewards_service_->RemoveObserver(this);
}

void RewardsDonateDOMHandler::Init() {
  Profile* profile = Profile::FromWebUI(web_ui());
  rewards_service_ =
      brave_rewards::RewardsServiceFactory::GetForProfile(profile);
  if (rewards_service_)
    rewards_service_->AddObserver(this);
}

void RewardsDonateDOMHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback("getPublisherDonateData",
                                    base::BindRepeating(&RewardsDonateDOMHandler::GetPublisherDonateData,
                                                        base::Unretained(this)));
}

void RewardsDonateDOMHandler::GetPublisherDonateData(const base::ListValue* args) {
  std::string publisher_key;
  args->GetString(0, &publisher_key);
  LOG(ERROR) << "GET PUBLISHER" << publisher_key;
}

}  // namespace

BraveDonateUI::BraveDonateUI(content::WebUI* web_ui, const std::string& name)
    : ConstrainedWebDialogUI(web_ui) {
  Profile* profile = Profile::FromWebUI(web_ui);

  content::WebUIDataSource* data_source = CreateBasicUIHTMLSource(profile,
                                              name,
                                              kDonateJS,
                                              IDR_BRAVE_DONATE_JS,
                                              IDR_BRAVE_DONATE_HTML);
  content::WebUIDataSource::Add(profile, data_source);

  auto handler_owner = std::make_unique<RewardsDonateDOMHandler>();
  RewardsDonateDOMHandler * handler = handler_owner.get();
  web_ui->AddMessageHandler(std::move(handler_owner));
  handler->Init();
}

BraveDonateUI::~BraveDonateUI() {
}
