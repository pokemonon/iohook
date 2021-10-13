import { ChildProcess } from 'child_process';

import { eventToP } from '@pokemonon/knife';

export const inTS = __filename.endsWith('.ts');

export const createIPC = (proc: NodeJS.Process | ChildProcess) => {
    return eventToP({
        onAdapter(cb) {
            function callback(payload) {
                const { eventName, data } = payload;
                cb(eventName, data, data);
            }
            proc.on('message', callback);
        },
        emitAdapter(info) {
            const {
                eventName,
                data,
            } = info;
            proc.send?.({
                eventName,
                data,
            });
        },
    });
};