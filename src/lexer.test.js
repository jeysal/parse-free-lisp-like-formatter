const tokenize = require('./lexer');

test('recognizes all regular token types', () => {
  expect(
    tokenize(`
      (lambda id1 &id2
        (= true 123))
  `),
  ).toMatchSnapshot();
});

test('recognizes line comments', () => {
  expect(
    tokenize(`
      (a ;comment
      b)
  `),
  ).toMatchSnapshot();
});

test('recognizes block comments', () => {
  expect(
    tokenize(`
      (a #|com
      ment|# b)
  `),
  ).toMatchSnapshot();
});

test('generates tokens for empty lines', () => {
  expect(
    tokenize(`
      (a

      b)
    `),
  ).toMatchSnapshot();
});

test('treats a line as empty even if it contains whitespace', () => {
  expect(
    tokenize(`
      (a
\t
      b)
    `),
  ).toMatchSnapshot();
});

test('throws if the code includes an illegal char', () => {
  expect(() => tokenize('(a # b)')).toThrowError();
});
