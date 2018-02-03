const MAX_LINE_LENGTH = 80;

const allowsSpaceBefore = type => type !== 'rightPar';
const allowsSpaceAfter = type => !['leftPar', 'prefix'].includes(type);

const print = tokens => {
  let code = '';

  let nestingLevel = 0;
  let lineLength = 0;

  let consecutiveBreaks = 2;
  let prevAllowsSpace = false;

  // helpers
  const breakLine = () => {
    code += '\n';
    lineLength = 0;
    consecutiveBreaks++;
    prevAllowsSpace = false; // line break replaces space
  };

  // main loop
  for (const { type, value } of tokens) {
    let spaceBefore = prevAllowsSpace && allowsSpaceBefore(type);

    // soft line breaks
    let printLength = value.length + spaceBefore; // (spaceBefore ? 1 : 0)

    if (lineLength + printLength > MAX_LINE_LENGTH) {
      breakLine();

      // No space after all, just the break
      if (spaceBefore) printLength--;
      spaceBefore = false;
    }
    lineLength += printLength;

    // spacing
    if (spaceBefore) {
      code += ' ';
    }

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

print.MAX_LINE_LENGTH = MAX_LINE_LENGTH;

module.exports = print;
