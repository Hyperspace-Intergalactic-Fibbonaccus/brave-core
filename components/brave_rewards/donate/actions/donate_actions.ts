/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import { action } from 'typesafe-actions'

// Constant
import { types } from '../constants/donate_types'

export const onCloseDialog = () => action(types.ON_CLOSE_DIALOG)

export const onPublisherDonateData = (data: RewardsDonate.Publisher) => action(types.ON_CLOSE_DIALOG, {
  data
})
