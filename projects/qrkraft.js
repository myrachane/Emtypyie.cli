const chalk = require('chalk');

const project = {
  name: 'QR KRAFT',
  version: 'v1.01',
  repo: 'myrachane/QRkRAFT',
  description: 'Custom QR Code Generator with full styling',
  download: 'https://github.com/myrachane/QRkRAFT/releases/download/v1.01/QR.KRAFT.Setup.1.0.1.exe',
  filename: 'QR.KRAFT.Setup.1.0.1.exe',
  postInstall: (dest) => {
    console.log(`  ${chalk.dim('Run /qrkraft to launch the installer.')}`);
  },
  info: `
  ${chalk.bold('QR KRAFT')} - Custom QR Code Generator
  ${chalk.dim('─────────────────────────────────')}

  Module Shapes: Square, Circle, X, Plus
  Finder Styles: Standard, Rounded, Circle, Diamond, Leaf, Mixed, Frame, Dots
  Colors: Custom foreground, background, transparent support
  Logo: Overlay any image in the center
  Export: PNG and SVG

  ${chalk.dim('Support:')}  https://github.com/myrachane/QRkRAFT/issues
  ${chalk.dim('Docs:')}    https://github.com/myrachane/QRkRAFT#readme
  `
};

module.exports = project;
