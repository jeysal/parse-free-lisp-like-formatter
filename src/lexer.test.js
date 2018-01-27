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
