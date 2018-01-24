const tokenize = require('./lexer');

test('recognizes all token types', () => {
  expect(
    tokenize(`
      (lambda id1 &id2
        (= true 123))
  `),
  ).toMatchSnapshot();
});
