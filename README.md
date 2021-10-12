# WIP

## instruction
Implementing keyboard and mouse events listening by node.js

## usage

### download
```
yarn add @pokemonon/iohook
```

### api
```
import { iohook } from '@pokemonon/iohook'

iohook.on(eventName, (eventData) => {})
iohook.start() // Promise
```

| eventName    | eventData |
| ------------ | --------- |
| keydown      |           |
| keyup        |           |
| keypress     |           |
| mousedown    |           |
| mouseup      |           |
| click        |           |
| mousemove    |           |
| mousedragged |           |
| mousewheel   |           |