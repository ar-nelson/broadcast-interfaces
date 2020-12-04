export interface BroadcastInterface {
  readonly name: string;
  readonly address: string;
  readonly broadcast: string;
  readonly running: boolean;
  readonly internal: boolean;
}

export default function broadcastInterfaces(): readonly BroadcastInterface[];
