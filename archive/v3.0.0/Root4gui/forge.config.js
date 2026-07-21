const { MakerSquirrel } = require('@electron-forge/maker-squirrel');
const { MakerZIP } = require('@electron-forge/maker-zip');

module.exports = {
  packagerConfig: {
    name: 'emtypyie.cli-Wrapper',
    executableName: 'emtypyieWrapper',
    asar: true,
    icon: './src/renderer/logo',
    extraResource: ['./resources/emtypyie.exe']
  },
  rebuildConfig: {},
  makers: [
    new MakerSquirrel({ name: 'emtypyieWrapper' }),
    new MakerZIP({}, ['win32'])
  ],
  plugins: [],
  hooks: {}
};
