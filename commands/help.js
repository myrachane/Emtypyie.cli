const chalk = require('chalk');

function show(projects) {
  console.log();
  console.log(chalk.bold('  Available Commands:'));
  console.log();
  console.log(chalk.cyan('  get <project>') + chalk.dim('   - Install/download a project'));
  console.log(chalk.cyan('  help') + chalk.dim('          - Show this help'));
  console.log(chalk.cyan('  exit') + chalk.dim('          - Exit the CLI'));
  console.log();
  console.log(chalk.bold('  Available Projects:'));
  console.log();

  for (const [name, proj] of Object.entries(projects)) {
    const pad = ' '.repeat(Math.max(0, 12 - name.length));
    console.log(`  ${chalk.cyan(name)}${pad}${chalk.dim(proj.description || '')}`);
  }

  console.log();
}

module.exports = { show };
