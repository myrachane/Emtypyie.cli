const fs = require('fs');
const path = require('path');

async function playStartupAnimation(options = {}) {
  const { 
    totalMs = 2000, 
    lineCount = 300, 
    skip = false 
  } = options;
  
  if (skip || process.env.EMTYPYIE_NO_ANIM) return;
  
  const animPath = path.join(__dirname, '..', '..', 'startup_animation.json');
  if (!fs.existsSync(animPath)) {
    console.error('Animation file not found:', animPath);
    return;
  }
  
  const data = JSON.parse(fs.readFileSync(animPath, 'utf8'));
  const lines = data.lines;
  
  const timing = data.timing || {};
  const actualTotalMs = timing.total_ms || totalMs;
  const actualLineCount = timing.line_count || lineCount;
  const intervalMs = actualLineCount > 0 ? actualTotalMs / actualLineCount : 6.67;
  
  const termWidth = process.stdout.columns || 80;
  const targetWidth = Math.max(40, Math.min(termWidth, Math.floor(termWidth * 0.65)));
  
  for (let i = 0; i < actualLineCount; i++) {
    const line = lines[i % lines.length];
    const padded = line.length >= targetWidth 
      ? line.slice(0, targetWidth) 
      : line + ' '.repeat(targetWidth - line.length);
    
    process.stdout.write(padded + '\n');
    
    if (intervalMs > 0) {
      await new Promise(r => setTimeout(r, intervalMs));
    }
  }
}

module.exports = { playStartupAnimation };