const MAX_LINE_LENGTH = 80;
const INDENT_SIZE = 2;

const allowsSpaceBefore = type => type !== 'rightPar';
const allowsSpaceAfter = type => !['leftPar', 'prefix'].includes(type);

const print = tokens => {
  let code = '';

  let nestingLevel = 0;
  let lineLength = 0;
  let maxLineLength = MAX_LINE_LENGTH;

  let consecutiveBreaks = 2;
  let prevAllowsSpace = false;

  // helpers
  const breakLine = () => {
    code += '\n';
    consecutiveBreaks++;
    prevAllowsSpace = false; // line break replaces space
  };

  const softBreak = () => {
    breakLine();

    const indentSize = nestingLevel * INDENT_SIZE;
    const indentation = Array(indentSize)
      .fill(' ')
      .join('');

    code += indentation;
    lineLength = indentSize;
  };
  const hardBreak = () => {
    breakLine();

    lineLength = 0;
    maxLineLength = MAX_LINE_LENGTH;
  };

  // main loop
  for (const { type, value } of tokens) {
    let spaceBefore = prevAllowsSpace && allowsSpaceBefore(type);

    // nesting decrement needs to happen before print
    if (type === 'rightPar') nestingLevel--;

    // soft line breaks
    let printLength = value.length + spaceBefore; // (spaceBefore ? 1 : 0)

    if (lineLength + printLength > maxLineLength) {
      softBreak();

      // No space after all, just the break
      if (spaceBefore) printLength--;
      spaceBefore = false;
    }
    lineLength += printLength;
    maxLineLength = Math.max(lineLength, maxLineLength);

    // spacing
    if (spaceBefore) {
      code += ' ';
    }

    // print token
    code += value;

    // set previous information for next iteration
    prevAllowsSpace = allowsSpaceAfter(type);

    // hard line break
    if (type === 'emptyLine') while (consecutiveBreaks < 2) hardBreak();
    else consecutiveBreaks = 0;

    if (type === 'leftPar') nestingLevel++;
    if (type === 'rightPar' && nestingLevel === 0) hardBreak();
  }

  return code;
};

print.MAX_LINE_LENGTH = MAX_LINE_LENGTH;

module.exports = print;
