sps
===

solids pressure sensitive buttons

![img](http://i.imgur.com/ZL2aXJU.jpg)


## brain
[This is](https://github.com/solids/sps/blob/master/brain/brain.ino) an arduino sketch running on an arduino uno

## circuit

[This is] (https://github.com/solids/sps/tree/master/circuit) the latest board/schematic sensor design (designed in eagle)

## node

[This is] (https://github.com/solids/sps/tree/master/node) the code that runs on the physical board defined in the circuit folder.

Currently I2C is used for the bus, but we may migrate to chained SPI for less protocol
overhead.

## License

[MIT](http://tmpvar.mit-license.org)
