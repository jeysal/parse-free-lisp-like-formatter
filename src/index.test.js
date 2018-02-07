const format = require('.');

test('example algorithms', () => {
  expect(
    format(`
    ; factorial
    (defun factorial(n)(if(=n 0)1(*n(factorial(-n 1)))))

    ; fibonacci
    (defun fib(n)(if(lte n 2)1(+(fib(-n 1))(fib(-n 2)))))

    ; birthday paradox, already well-formatted
    (defconstant yearsize 365)
    (defun birthdayparadox (probability numberofpeople)
      (let ((newprobability (* (/
          (- yearsize numberofpeople) yearsize) probability)))
        (if (lt newprobability (/ 1 2))
            (1+ numberofpeople)
            (birthdayparadox newprobability
                             (1+ numberofpeople)))))
      `),
  ).toMatchSnapshot();
});
