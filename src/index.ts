import * as path from 'path'
import { EventEmitter } from 'events'
import { ChildProcess, spawn } from 'child_process'
import { createIPC, inTS } from './utils';
import { Event } from './hook-process'

class IOHook extends EventEmitter {
    #hookProc!: ChildProcess
    
    async start() {
        if (this.#hookProc) return;

        const hookProc = this.#hookProc = spawn(inTS ? 'ts-node' : 'node', ['./hook-process'], {
            stdio: ['ignore', 'ignore', 'ignore', 'ipc'],
            // stdio: ['inherit', 'inherit', 'inherit', 'ipc'],
            cwd: path.dirname(__filename),
        })

        return new Promise<void>((resolve) => {
            const ipc = createIPC(hookProc);

            ipc.on('iohook-event', ((_, event: Event) => {
                if (event.type === 'enabled') {
                    resolve();
                    return;
                }
                this.emit(event.type, event.data)
            }) as any)
        })
    }
}

const iohook = new IOHook();

export { iohook }
