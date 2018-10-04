/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import { Reducer } from 'redux'

// Constant
import { types } from '../constants/donate_types'

export const defaultState: RewardsDonate.State = {
  finished: false,
  error: false,
  publisher: undefined
}

const publishersReducer: Reducer<RewardsDonate.State> = (state: RewardsDonate.State = defaultState, action) => {
  switch (action.type) {
    case types.ON_CLOSE_DIALOG:
      chrome.send('dialogClose')
      break
  }

  return state
}

export default publishersReducer
