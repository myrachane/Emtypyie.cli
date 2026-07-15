#!/usr/bin/env node

const readline = require('readline');
const gradient = require('gradient-string');
const chalk = require('chalk');
const getCommand = require('./commands/get');
const helpCommand = require('./commands/help');

const BANNER = `
  ███████╗███╗   ███╗████████╗██╗██╗   ██╗██████╗ ██╗███████╗
  ██╔════╝████╗ ████║╚══██╔══╝██║╚██╗ ██╔╝██╔══██╗██║██╔════╝
  █████╗  ██╔████╔██║   ██║   ██║ ╚████╔╝ ██████╔╝██║█████╗
  ██╔══╝  ██║╚██╔╝██║   ██║   ██║  ╚██╔╝  ██╔═══╝ ██║██╔══╝
  ███████╗██║ ╚═╝ ██║   ██║   ██║   ██║   ██║     ██║███████╗
  ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚═╝   ╚═╝   ╚═╝     ╚═╝╚══════╝
`;

const QUOTE = `  "${chalk.italic('code. create. conquer.')}"`;

const projects = {
  qrkraft: require('./projects/qrkraft')
};

async function prompt(rl, question) {
  return new Promise((resolve) => {
    rl.question(question, resolve);
  });
}

async function interactive() {
  const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    prompt: gradient.pastel('>> ')
  });

  console.clear();
  console.log(gradient.pastel.multiline(BANNER));
  console.log(QUOTE);
  console.log();
  console.log(chalk.dim('  Type ') + chalk.cyan('get <project>') + chalk.dim(' to install a project'));
  console.log(chalk.dim('  Type ') + chalk.cyan('help') + chalk.dim(' for available commands'));
  console.log(chalk.dim('  Type ') + chalk.cyan('exit') + chalk.dim(' or press Ctrl+C to quit'));
  console.log();

  rl.prompt();

  rl.on('line', async (line) => {
    const input = line.trim();
    const parts = input.split(/\s+/);
    const cmd = parts[0]?.toLowerCase();
    const arg = parts.slice(1).join(' ');

    if (!cmd || cmd === '') {
      rl.prompt();
      return;
    }

    if (cmd === 'exit' || cmd === 'quit') {
      console.log(chalk.dim('\n  Goodbye.'));
      rl.close();
      process.exit(0);
      return;
    }

    if (cmd === 'help') {
      helpCommand.show(projects);
    } else if (cmd === 'get') {
      if (!arg) {
        console.log(chalk.red('  Specify a project name. Usage: get <project>'));
        console.log(chalk.dim('  Available: ') + Object.keys(projects).join(', '));
      } else if (projects[arg.toLowerCase()]) {
        await getCommand.install(arg.toLowerCase(), projects[arg.toLowerCase()]);
      } else {
        console.log(chalk.red(`  Unknown project "${arg}".`));
        console.log(chalk.dim('  Available: ') + Object.keys(projects).join(', '));
      }
    } else {
      console.log(chalk.red(`  Unknown command "${cmd}". Type help for available commands.`));
    }

    rl.prompt();
  });

  rl.on('close', () => {
    console.log(chalk.dim('\n  Goodbye.'));
    process.exit(0);
  });
}

async function direct(args) {
  const cmd = args[0]?.toLowerCase();

  if (cmd === 'help' || !cmd) {
    helpCommand.show(projects);
    return;
  }

  if (cmd === 'get') {
    const projectName = args[1]?.toLowerCase();
    if (!projectName) {
      console.log(chalk.red('Specify a project name. Usage: emtypyie get <project>'));
      process.exit(1);
    }
    if (!projects[projectName]) {
      console.log(chalk.red(`Unknown project "${projectName}".`));
      process.exit(1);
    }
    await getCommand.install(projectName, projects[projectName]);
    return;
  }

  console.log(chalk.red(`Unknown command "${cmd}".`));
  process.exit(1);
}

const args = process.argv.slice(2);

if (args.length === 0) {
  interactive();
} else {
  direct(args);
}
