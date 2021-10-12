import { eventToP } from '@pokemonon/knife';
import { ChildProcess } from 'child_process';
import { Readable } from 'stream';
import { Worker, WorkerOptions } from 'worker_threads'

export const createWorkerTS = (filename: string, options: WorkerOptions = {}) => {
    const source = `
        const wt = require('worker_threads');
        require('ts-node').register();
        require('${filename}');
    `

    const workerData = options.workerData || {};
    return new Worker(source, {
        ...options,
        eval: true,
    })
}

export const inTS = __filename.endsWith('.ts')

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
                data
            });
        },
    });
}