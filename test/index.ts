import { iohook } from '../src'

iohook.on('mousemove', (e) => {
    console.log('mousemove', e)
})
iohook.on('mousedown', (e) => {
    console.log('mousedown1', e)
})
iohook.on('mousedown', (e) => {
    console.log('mousedown2', e)
})
iohook.on('keydown', (e) => {
    console.log('keydown', e)
})
iohook.on('keyup', (e) => {
    console.log('keyup', e)
})
iohook.once('click', (e) => {
    console.log('click', e)
})

iohook.start();