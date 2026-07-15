const chalk = require('chalk');

const retro = chalk.hex('#33ff33');
const retroDim = chalk.hex('#1a7a1a');
const retroAccent = chalk.hex('#66ff66');

function show(projects) {
  console.log();
  console.log(retroDim('  ─── Commands ───'));
  console.log();
  console.log(retro('  get <project>') + retroDim('   install a project'));
  console.log(retro('  help') + retroDim('          this screen'));
  console.log(retro('  exit') + retroDim('          quit'));
  console.log();
  console.log(retroDim('  ─── Projects ───'));
  console.log();

  for (const [name, proj] of Object.entries(projects)) {
    const pad = ' '.repeat(Math.max(0, 12 - name.length));
    console.log(`  ${retroAccent(name)}${pad}${retroDim(proj.description || '')}`);
  }

  console.log();
  console.log(retroDim('  ─────────────────────'));
  console.log();
}

module.exports = { show };
