declare namespace RewardsDonate {
  interface State {
    publisher?: Publisher
    finished: boolean
    error: boolean
  }

  interface ApplicationState {
    rewardsDonateData: State | undefined
  }

  interface ComponentProps {
    actions: any
    rewardsDonateData: State
  }

  interface Publisher {
    publisher_key: string
  }
}
