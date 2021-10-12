import bindings from 'bindings'
import { createIPC } from './utils';

export interface IOHookNode {
    start(callback: any): void;
}

export enum EventTypeEnum {
    enabled = 'enabled',
    disabled = 'disabled',
    keydown = 'keydown',
    keyup = 'keyup',
    keytyped = 'keytyped',
    mousedown = 'mousedown',
    mouseup = 'mouseup',
    click = 'click',
    mousemove = 'mousemove',
    mousedragged = 'mousedragged',
    mousewheel = 'mousewheel'
}
export type EventType = keyof typeof EventTypeEnum
export interface KeyboardEventData {
    keycode: number;
    rawcode: number;
    keychar: number;
}
export interface MouseEventData {
    button: number;
    clicks: number;
    x: number;
    y: number;
}
export interface MouseWheelEventData {
    clicks: number;
    x: number;
    y: number;
    type: number;
    amount: number;
    rotation: number;
    direction: number;
}
export interface KeyboardEvent {
    type: EventTypeEnum.keydown | EventTypeEnum.keyup | EventTypeEnum.keytyped;
    data: KeyboardEventData;
}
export interface MouseEvent {
    type: EventTypeEnum.mousedown | EventTypeEnum.mouseup | EventTypeEnum.click | EventTypeEnum.mousemove | EventTypeEnum.mousedragged;
    data: MouseEventData;
}
export interface MouseWheelEvent {
    type: EventTypeEnum.mousewheel;
    data: MouseWheelEventData;
}
export interface Enabled {
    type: EventTypeEnum.enabled;
}
// export interface Disabled {
//     type: EventTypeEnum.disabled;
// }
export type Event = KeyboardEvent | MouseEvent | MouseWheelEvent | Enabled;

const iohookNode: IOHookNode = bindings('iohook.node');
const ipc = createIPC(process);

export interface HookProcessIPCEvents {
    'iohook-event': Event
}

iohookNode.start((evt) => {
    ipc.emit('iohook-event', evt)
})
