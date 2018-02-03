const allowsSpaceBefore = type => type !== 'rightPar';
const allowsSpaceAfter = type => !['leftPar', 'prefix'].includes(type);

const print = tokens => {
  let code = '';
  let nestingLevel = 0;
  let prevAllowsSpace = false;
  let consecutiveBreaks = 2;

  // helpers
  const breakLine = () => {
    code += '\n';
    consecutiveBreaks++;
    prevAllowsSpace = false; // line break replaces space
  };

  // main loop
  for (const { type, value } of tokens) {
    if (prevAllowsSpace && allowsSpaceBefore(type)) code += ' ';

    // print token
    code += value;

    // set previous information for next iteration
    prevAllowsSpace = allowsSpaceAfter(type);

    // hard line break
    if (type === 'emptyLine') while (consecutiveBreaks < 2) breakLine();
    else consecutiveBreaks = 0;

    if (type === 'leftPar') nestingLevel++;
    if (type === 'rightPar' && --nestingLevel === 0) breakLine();
  }

  return code;
};

module.exports = print;
