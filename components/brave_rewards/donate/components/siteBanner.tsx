/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { bindActionCreators, Dispatch } from 'redux'
import { connect } from 'react-redux'

// Components
import { SiteBanner } from 'brave-ui/features/rewards'

// Utils
import * as donateActions from '../actions/donate_actions'

interface Props extends RewardsDonate.ComponentProps {
}

class Banner extends React.Component<Props, {}> {
  doNothing () {
    console.log('click')
  }

  get actions () {
    return this.props.actions
  }

  onClose = () => {
    this.actions.onCloseDialog()
  }

  render () {
    const donationAmounts = [
      { tokens: 1, converted: 0.3, selected: false },
      { tokens: 5, converted: 1.5, selected: false },
      { tokens: 10, converted: 3, selected: false }
    ]

    return (
      <SiteBanner
        domain={'duckduckgo.com'}
        title={''}
        currentDonation={0}
        balance={5}
        bgImage={undefined}
        logo={undefined}
        donationAmounts={donationAmounts}
        logoBgColor={''}
        onDonate={this.doNothing}
        onAmountSelection={this.doNothing}
        currentAmount={5}
        onClose={this.onClose}
        social={[
          {
            type: 'twitter',
            name: '@DuckDuckGo',
            handler: 'DuckDuckGo'
          },
          {
            type: 'youtube',
            name: 'duckduckgo',
            handler: 'UCm_TyecHNHucwF_p4XpeFkQ'
          },
          {
            type: 'twitch',
            name: 'duckDuckGo',
            handler: 'duckduckgo'
          }
        ]}
      />
    )
  }
}

const mapStateToProps = (state: RewardsDonate.ApplicationState) => ({
  rewardsDonateData: state.rewardsDonateData
})

const mapDispatchToProps = (dispatch: Dispatch) => ({
  actions: bindActionCreators(donateActions, dispatch)
})

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(Banner)
