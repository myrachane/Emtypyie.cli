const t = require('./theme');

function show(projects) {
  console.log();
  console.log(t.retroDim('  ─── Commands ───'));
  console.log();
  console.log(t.retro('  /get <project>') + t.retroDim('     install a project'));
  console.log(t.retro('  /flash <project>') + t.retroDim('   re-download latest version'));
  console.log(t.retro('  /info <project>') + t.retroDim('    show project details'));
  console.log(t.retro('  /rm <project>') + t.retroDim('      delete project files'));
  console.log(t.retro('  /issue <project>') + t.retroDim('   open issue tracker'));
  console.log(t.retro('  /issue <project> -m') + t.retroDim('  file a bug report'));
  console.log(t.retro('  /bakafetch') + t.retroDim('         system info with style'));
  console.log(t.retro('  /bf') + t.retroDim('               shortcut for /bakafetch'));
  console.log(t.retro('  /wrap bakafetch <c>') + t.retroDim('  change bakafetch color'));
  console.log(t.retro('  /about') + t.retroDim('             about emtypyie'));
  console.log(t.retro('  /wiki') + t.retroDim('              open wiki.emtypyie.in'));
  console.log(t.retro('  /help') + t.retroDim('             this screen'));
  console.log(t.retro('  /exit') + t.retroDim('             quit'));
  console.log();
  console.log(t.retroDim('  ─── Projects ───'));
  console.log();
  for (const [name, proj] of Object.entries(projects)) {
    const pad = ' '.repeat(Math.max(0, 12 - name.length));
    console.log(`  ${t.retroAccent(name)}${pad}${t.retroDim(proj.description || '')}`);
  }
  console.log();
}

module.exports = { show };
