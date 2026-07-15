const os = require('os');
const { execSync } = require('child_process');
const chalk = require('chalk');

const ASCII_ARTS = [
  [
    '⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣄⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀',
    '⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠳⣄⠀⠀⠀⠀⠀⠀⠀⠀',
    '⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀',
    '⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣠⣤⣤⣤⣤⣄⣀⣀⠀⠀⠀⠀⠀⣠⠎⠀⠀⠀⠀⠀⠀⠀⠀',
    '⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣖⡉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀',
    '⠀⠀⢠⣄⣀⣠⣤⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀',
    '⠀⠀⠀⠉⠻⣿⣿⣿⣿⣿⣿⣯⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀',
    '⠀⠀⠀⠀⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀',
    '⠀⠀⠀⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀',
    '⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⠀',
    '⠀⠀⣰⣿⣿⣿⣿⣿⣿⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣄⠀⠀',
    '⠠⣾⣿⢿⣿⣿⣿⣿⡿⠁⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠉⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⠉⠀⠀',
    '⠀⠀⠀⢸⣿⣿⣿⡿⠑⠊⣿⣿⡿⠿⠛⠛⠙⠛⣻⣿⣿⣄⡻⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀',
    '⠀⠀⠀⢸⣿⣿⣿⡗⠾⠛⠉⠉⠀⠀⠀⠀⠀⠀⠈⠉⠉⠙⠛⠛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀',
    '⠀⠀⠀⢸⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⠟⠛⠻⣿⣿⣿⣿⣿⣿⡄⠀',
    '⠀⠀⠀⠀⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⣀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⢶⡋⠳⢸⣿⣿⣿⣿⣿⣇⠀',
    '⠀⠂⠀⠀⠘⣿⣿⣿⡀⠀⠀⠀⠀⠀⠉⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⡗⠚⢁⣠⣾⣿⣿⣿⣿⣿⣿⠀',
    '⠀⠉⠀⠀⠀⠈⣻⣿⣿⣦⣄⣀⡀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣾⣿⣿⣿⣷⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄',
    '⠀⠀⠀⢺⣿⠤⠿⢿⣿⣿⣿⣿⣿⣿⣷⣶⡄⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇',
    '⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⢿⣿⣿⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇',
    '⠀⠀⠀⠀⠀⠀⣸⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁⠀⣀⡠⠜⠋⠁⠀⠀⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠁',
    '⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⡿⠛⣠⣟⣁⠤⠖⠋⠁⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀',
    '⠀⠀⠀⠀⠀⠀⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⡟⢸⠿⠃⠀',
    '⠀⠀⠀⠀⠀⠀⢸⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢦⠀⠀⠀⠀⠀⠀⠀',
    '⠀⠀⠀⠀⠀⠀⡼⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣆⠀⠀⠀⠀⠀⠀',
  ],
  [
    '⡏⠉⠉⠉⠉⠉⠉⠋⠉⠉⠉⠉⠉⠉⠋⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠙⠉⠉⠉⠹',
    '⡇⢸⣿⡟⠛⢿⣷⠀⢸⣿⡟⠛⢿⣷⡄⢸⣿⡇⠀⢸⣿⡇⢸⣿⡇⠀⢸⣿⡇⠀',
    '⡇⢸⣿⣧⣤⣾⠿⠀⢸⣿⣇⣀⣸⡿⠃⢸⣿⡇⠀⢸⣿⡇⢸⣿⣇⣀⣸⣿⡇⠀',
    '⡇⢸⣿⡏⠉⢹⣿⡆⢸⣿⡟⠛⢻⣷⡄⢸⣿⡇⠀⢸⣿⡇⢸⣿⡏⠉⢹⣿⡇⠀',
    '⡇⢸⣿⣧⣤⣼⡿⠃⢸⣿⡇⠀⢸⣿⡇⠸⣿⣧⣤⣼⡿⠁⢸⣿⡇⠀⢸⣿⡇⠀',
    '⣇⣀⣀⣀⣀⣀⣀⣄⣀⣀⣀⣀⣀⣀⣀⣠⣀⡈⠉⣁⣀⣄⣀⣀⣀⣠⣀⣀⣀⣰',
    '⣇⣿⠘⣿⣿⣿⡿⡿⣟⣟⢟⢟⢝⠵⡝⣿⡿⢂⣼⣿⣷⣌⠩⡫⡻⣝⠹⢿⣿⣷',
    '⡆⣿⣆⠱⣝⡵⣝⢅⠙⣿⢕⢕⢕⢕⢝⣥⢒⠅⣿⣿⣿⡿⣳⣌⠪⡪⣡⢑⢝⣇',
    '⡆⣿⣿⣦⠹⣳⣳⣕⢅⠈⢗⢕⢕⢕⢕⢕⢈⢆⠟⠋⠉⠁⠉⠉⠁⠈⠼⢐⢕⢽',
    '⡗⢰⣶⣶⣦⣝⢝⢕⢕⠅⡆⢕⢕⢕⢕⢕⣴⠏⣠⡶⠛⡉⡉⡛⢶⣦⡀⠐⣕⢕',
    '⡝⡄⢻⢟⣿⣿⣿⣷⣕⣕⣅⣿⣔⣕⣵⣵⣿⣿⢠⣿⢠⣮⡈⣌⠨⠅⠹⣷⡀⢱⢕',
    '⡝⡵⠟⠈⢀⣀⣀⡀⠉⢿⣿⣿⣿⣿⣿⣿⣿⣼⣿⢈⡋⠴⢿⡟⣡⡇⣿⡇⡀⢕',
    '⡝⠁⣠⣾⠟⡉⡉⡉⠻⣦⣻⣿⣿⣿⣿⣿⣿⣿⣿⣧⠸⣿⣦⣥⣿⡇⡿⣰⢗⢄',
    '⠁⢰⣿⡏⣴⣌⠈⣌⠡⠈⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣬⣉⣉⣁⣄⢖⢕⢕⢕',
    '⡀⢻⣿⡇⢙⠁⠴⢿⡟⣡⡆⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣵⣵⣿',
    '⡻⣄⣻⣿⣌⠘⢿⣷⣥⣿⠇⣿⣿⣿⣿⣿⣿⠛⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿',
    '⣷⢄⠻⣿⣟⠿⠦⠍⠉⣡⣾⣿⣿⣿⣿⣿⣿⢸⣿⣦⠙⣿⣿⣿⣿⣿⣿⣿⣿⠟',
    '⡕⡑⣑⣈⣻⢗⢟⢞⢝⣻⣿⣿⣿⣿⣿⣿⣿⠸⣿⠿⠃⣿⣿⣿⣿⣿⣿⡿⠁⣠',
    '⡝⡵⡈⢟⢕⢕⢕⢕⣵⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣶⣿⣿⣿⣿⣿⠿⠋⣀⣈⠙',
    '⡝⡵⡕⡀⠑⠳⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⢉⡠⡲⡫⡪⡪⡣',
  ],
  [
    '    ⠀⠀⠀⠀⠀⢀⣠⣤⣶⣶⣿⣿⣿⣿⣷⣶⣤⣀⡀⠀⠀⠀⠀⠀⠀',
    '    ⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀',
    '    ⠀⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡀⠀⠀',
    '    ⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠻⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀',
    '    ⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿⡇',
    '    ⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣀⣾⣿⣿⣿⣿⣿⣿⣿⣿⡇',
    '    ⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇',
    '    ⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇',
    '    ⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇',
    '    ⢸⣿⣿⣿⣿⣿⡿⢿⣿⣿⣿⣿⡄⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇',
    '    ⢸⣿⣿⣿⣿⡟⠀⠈⢻⣿⣿⣿⣇⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇',
    '    ⢸⣿⣿⣿⣿⡇⠀⠀⢸⣿⣿⣿⣿⡀⢹⣿⣿⣿⣿⣿⣿⣿⣿⡇',
    '    ⢸⣿⣿⣿⣿⣷⣶⣶⣿⣿⣿⣿⣿⣷⣾⣿⣿⣿⣿⣿⣿⣿⣿⡇',
    '    ⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇',
    '    ⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀',
    '    ⠀⠀⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠀⠀',
    '    ⠀⠀⠀⠈⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⠁⠀⠀⠀',
    '    ⠀⠀⠀⠀⠀⠀⠈⠙⠛⠿⢿⣿⣿⣿⠿⠟⠛⠉⠀⠀⠀⠀⠀⠀',
  ],
  [
    '⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀',
    '⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿',
    '⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠛⠛⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿',
    '⣿⣿⣿⣿⣿⣿⣿⠏⠁⠀⠀⠀⠀⠀⠀⠈⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿',
    '⣿⣿⣿⣿⣿⣿⠃⠀⠀⢀⣀⣤⣤⣤⣀⡀⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿',
    '⣿⣿⣿⣿⣿⣿⠀⠀⣴⣿⣿⣿⣿⣿⣿⣿⣦⠀⣿⣿⣿⣿⣿⣿⣿⣿',
    '⣿⣿⣿⣿⣿⡇⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⢸⣿⣿⣿⣿⣿⣿⣿',
    '⣿⣿⣿⣿⣿⡇⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⢸⣿⣿⣿⣿⣿⣿⣿',
    '⣿⣿⣿⣿⣿⡇⠀⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⢸⣿⣿⣿⣿⣿⣿⣿',
    '⣿⣿⣿⣿⣿⣧⣀⠀⠙⠿⣿⣿⣿⣿⣿⠿⠋⠀⣸⣿⣿⣿⣿⣿⣿⣿',
    '⣿⣿⣿⣿⣿⣿⣿⣷⣦⣄⣀⡉⠉⣁⣀⣠⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿',
    '⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿',
    '⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿',
    '⣿⠉⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⣿',
    '⣿⠀⠘⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠀⣿',
    '⣿⡇⠀⠀⠉⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠉⠀⠀⢸⣿',
    '⣿⣿⣦⣄⡀⠀⠀⠀⠉⠙⠛⠛⠛⠛⠛⠛⠋⠉⠀⠀⠀⢀⣠⣴⣿⣿',
    '⣿⣿⣿⣿⣿⣿⣶⣶⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣴⣶⣿⣿⣿⣿⣿⣿',
    '⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿',
    '⡿⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠻',
  ],
];

const TSUNDERE_LINES = [
  'B-baka! It\'s not like I want to show you my system info or anything!',
  'I-It\'s not like I did this for you, okay?!',
  'Hmph! Don\'t get the wrong idea, idiot!',
  'You better appreciate this, or else!',
  'S-s-stop staring! It\'s embarrassing!',
  'Tch. I just happened to have nothing better to do.',
  'Don\'t think this means I like you or anything, b-baka!',
  'F-fine! I\'ll show you... just this once!',
  'It\'s not like I spent time making this look good or anything!',
  'W-whatever! Here\'s your stupid system info!',
];

const COLORS = {
  cyan: '#00ffff',
  blue: '#4488ff',
  green: '#33ff33',
  red: '#ff4444',
  violet: '#aa66ff',
};

let activeColor = 'cyan';
let cachedGpu = null;

function getColor() {
  return activeColor;
}

function setColor(color) {
  const lower = color.toLowerCase();
  if (COLORS[lower]) {
    activeColor = lower;
    return true;
  }
  if (/^#[0-9a-f]{6}$/i.test(color)) {
    activeColor = color;
    return true;
  }
  if (/^[0-9a-f]{6}$/i.test(color)) {
    activeColor = '#' + color.toLowerCase();
    return true;
  }
  return false;
}

function getGpu() {
  if (cachedGpu) return cachedGpu;
  try {
    const output = execSync(
      'powershell -Command "Get-CimInstance Win32_VideoController | Select-Object -ExpandProperty Name"',
      { timeout: 5000, encoding: 'utf8', stdio: ['pipe', 'pipe', 'ignore'] }
    );
    cachedGpu = output.trim().split('\n').filter(Boolean).map(l => l.trim()).join(', ') || 'Unknown';
  } catch {
    cachedGpu = 'Unknown';
  }
  return cachedGpu;
}

function getOsName() {
  const platform = os.platform();
  const release = os.release();
  if (platform === 'win32') {
    const ver = parseFloat(release);
    if (ver >= 10) return 'Windows 11';
    if (ver >= 6.3) return 'Windows 8.1';
    if (ver >= 6.2) return 'Windows 8';
    if (ver >= 6.1) return 'Windows 7';
    return 'Windows';
  }
  if (platform === 'linux') return 'Linux';
  if (platform === 'darwin') return 'macOS';
  return platform;
}

function getBuild() {
  const p = os.platform();
  if (p === 'win32') return 'windows';
  if (p === 'linux') return 'linux';
  if (p === 'darwin') return 'macos';
  return p;
}

function formatUptime(seconds) {
  const days = Math.floor(seconds / 86400);
  const hours = Math.floor((seconds % 86400) / 3600);
  const mins = Math.floor((seconds % 3600) / 60);
  const parts = [];
  if (days > 0) parts.push(`${days}d`);
  if (hours > 0) parts.push(`${hours}h`);
  if (mins > 0 || parts.length === 0) parts.push(`${mins}m`);
  return parts.join(' ');
}

function fmtMem(mb) {
  if (mb >= 1024) return (mb / 1024).toFixed(1) + ' GiB';
  return mb.toFixed(0) + ' MiB';
}

function show() {
  const platform = os.platform();
  const release = os.release();
  const hostname = os.hostname();
  const uptime = os.uptime();
  const cpus = os.cpus();
  const totalMem = os.totalmem() / 1024 / 1024;
  const freeMem = os.freemem() / 1024 / 1024;
  const usedMem = totalMem - freeMem;
  const cpuModel = cpus[0]?.model || 'Unknown';
  const cpuCores = cpus.length;

  const color = getColor();
  const c = chalk.hex(color);
  const cDim = chalk.hex(color).dim;
  const cBold = chalk.hex(color).bold;

  const art = ASCII_ARTS[Math.floor(Math.random() * ASCII_ARTS.length)];
  const tsun = TSUNDERE_LINES[Math.floor(Math.random() * TSUNDERE_LINES.length)];

  const labelWidth = 10;

  const infoLines = [
    { label: 'OS', value: `${getOsName()} (${release})` },
    { label: 'HOST', value: hostname.toUpperCase() },
    { label: 'KERNEL', value: `emtypyie cli v${require('../package.json').version}` },
    { label: 'UPTIME', value: formatUptime(uptime) },
    { label: 'SHELL', value: 'emtypyie' },
    { label: 'CPU', value: `${cpuModel} (${cpuCores})` },
    { label: 'GPU', value: getGpu() },
    { label: 'MEMORY', value: `${fmtMem(usedMem)} / ${fmtMem(totalMem)}` },
    { label: 'BUILD', value: getBuild() },
  ];

  console.log();
  console.log(`  ${cDim('┌')}${cDim('─'.repeat(44))}${cDim('┐')}`);
  for (const l of art) {
    console.log(`  ${cDim('│')} ${c(l)}${cDim(' │')}`);
  }
  console.log(`  ${cDim('└')}${cDim('─'.repeat(44))}${cDim('┘')}`);

  const separator = cDim('  ──────────────────────────────────');
  console.log();
  console.log(cBold(`  EMTYPYIE`) + c(`@${hostname}`));
  console.log(separator);
  for (const l of infoLines) {
    const pad = ' '.repeat(Math.max(1, labelWidth - l.label.length));
    console.log(`  ${c(l.label)}${cDim('.'.repeat(4))}${pad}${cDim('·')}  ${c(l.value)}`);
  }
  console.log(separator);
  console.log();
  console.log(`  ${c.italic(tsun)}`);
  console.log();
}

function labelPad(label, width) {
  return label + '.'.repeat(Math.max(1, width - label.length));
}

module.exports = { show, setColor, getColor, COLORS };
