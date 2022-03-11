(defclass var()
    (
        (id :accessor var-id)
        (val :accessor var-val)
    )
)
(setf current-class nil)
(setf input-file nil)
(setf result nil)
(setf p-flag t)
(setf is-mult nil)
(setf float-candidate nil)
(setf is-comment nil)
(setf comment-candidate nil)
(setf leading-zero nil)
(setf leading-digit nil)
(setf leading-zero-error nil)
(setf leading-digit-error nil)
(setf variables '())
(setf indexes '())

(defconstant keywords '(AND OR NOT EQUAL LESS NIL LIST APPEND CONCAT SET DEFFUN FOR IF EXIT LOAD DISP TRUE FALSE DEFVAR))
(defconstant keyword-names '(KW_AND KW_OR KW_NOT KW_EQUAL KW_LESS KW_NIL KW_LIST KW_APPEND KW_CONCAT KW_SET KW_DEFFUN KW_FOR KW_IF KW_EXIT KW_LOAD KW_DISP KW_TRUE KW_FALSE KW_DEFVAR))
(defconstant operators '("+" "-" "/" "*" "(" ")" "**" "," "\'" "\""))
(defconstant operator-names '(OP_PLUS OP_MINUS OP_DIV OP_MULT OP_OP OP_CP OP_DBLMULT OP_COMMA OP_APOSTROPHE OP_OC OP_CC))
(defconstant LETTER 1)
(defconstant DIGIT 2)
(defconstant OTHER 3)

(defun getCharClass(C)
	(cond 
		((alpha-char-p C)
			LETTER
		)
		((not (null (digit-char-p C)))
			DIGIT
		)
		(t
			OTHER
		)
	)
)

(defun gppinterpreter(&optional file-name)
    (if (eq file-name nil)
        (setf input-file (open "input.g++"))
        (setf input-file (open file-name))
    )
    (setf lex-str (string ""))
    (loop for c = (read-char input-file nil)
        while c do 
        (progn
            (setf current-class (getCharClass c))
            (cond
                ((eq current-class OTHER)
                    (if (and (/= (length lex-str) 0) (not is-comment))
                        (tokenize lex-str)
                    )
                    (cond
                        ((char= c #\Newline)
                            (if (/= (length lex-str) 0)
                                (progn
                                    (tokenize lex-str)
                                    (start result)
                                )
                            )
                        )

                        (is-comment
                            (setf lex-str (concatenate 'string lex-str (string c)))
                        )

                        ((char= c #\")
                            (setf p-flag (not p-flag))
                            (tokenize c)
                        )

                        (is-mult
                            (if (char= c #\*)
                                (progn
                                    (setf token '())
                                    (setf token (append token (list "**")))
                                    (setf token (append token '(OP_DBLMULT)))
                                    (setf result (append result (list token)))
                                )
                                (progn 
                                    (setf token '())
                                    (setf token (append token (list "*")))
                                    (setf token (append token '(OP_MULT)))
                                    (setf result (append result (list token)))
                                    (if (not (numberp (position c '(#\Space #\Tab))))
                                        (tokenize c)
                                    )
                                )
                            )
                            (setf is-mult nil)
                        )

                        (comment-candidate
                            (if (char= c #\;)
                                (progn 
                                    (setf is-comment t)
                                    (setf comment-candidate nil)
                                    (setf lex-str (concatenate 'string lex-str ";;"))
                                )
                                (progn
                                    (setf comment-candidate nil)
                                    (setf token '())
                                    (setf token (append token (list ";")))
                                    (setf token (append token '(SYNTAX ERROR #\; CANNOT BE TOKENIZED)))
                                    (setf result (append result (list token)))
                                    (exitWithError "; Cannot Be Tokenized")
                                    (tokenize c)
                                )
                            )
                        )

                        ((char= c #\))
                            (tokenize c)
                            (if (isExpressionEnd result)
                                (start result)
                            )
                        )

                        ((char= c #\*)
                            (setf is-mult t)
                        )

                        ((char= c #\;)
                            (setf comment-candidate t)
                        )

                        (t
                            (if (not (numberp (position c '(#\Space #\Tab))))
                                (tokenize c)
                            )
                        )
                    )
                )
                ((eq current-class DIGIT)
                    (if is-mult
                        (progn
                            (setf token '())
                            (setf token (append token (list "*")))
                            (setf token (append token '(OP_MULT)))
                            (setf result (append result (list token)))
                            (setf is-mult nil)
                        )
                    )
                    (cond
                        (leading-zero
                            (setf leading-zero-error t)
                            (setf leading-zero nil)
                        )
                        ((= (length lex-str) 0)
                            (setf leading-digit t)
                            (if (char= c #\0)
                                (setf leading-zero t)
                            )
                        )
                    )
                    (setf lex-str (concatenate 'string lex-str (string c)))
                )
                ((eq current-class LETTER)
                    (if is-mult
                        (progn
                            (setf token '())
                            (setf token (append token (list "*")))
                            (setf token (append token '(OP_MULT)))
                            (setf result (append result (list token)))
                        )
                    )
                    (cond 
                        (leading-digit
                            (setf leading-digit-error t)
                            (setf leading-zero-error nil)
                            (setf leading-digit nil)
                        )
                    )
                    (setf lex-str (concatenate 'string lex-str (string c)))
                )
            )
        )
    )
    (if (/= (length lex-str) 0)
        (tokenize lex-str)
    )
    (if result
        (start result)
    )
    (close input-file)
)

(defun tokenize(lex-tokenizable)
    (setf general-error nil)
    (cond
        (is-comment
            (setf token-name '(COMMENT))
        )

        (leading-digit-error
            (setf token-name '(SYNTAX ERROR IDENTIFIERS CANNOT START WITH LEADING DIGITS))
        )

        (leading-zero-error
            (setf token-name '(SYNTAX ERROR VALUES CANNOT START WITH LEADING ZEROS))
        )

        (leading-digit
            (setf token-name '(VALUE))
        )

        ((stringp lex-tokenizable)
            (cond 
                ((isKeyword lex-tokenizable)
                    (setf token-name (list (nth index keyword-names)))
                )

                (t
                    (setf token-name '(IDENTIFIER))
                )
            )
        )

        ;; for character inputs
        (t
            (cond
                ((isOperator lex-tokenizable)
                    (if (and (not p-flag) (char= lex-tokenizable #\"))
                        (setf token-name (list (nth (+ 1 index) operator-names)))
                        (setf token-name (list (nth index operator-names)))
                    )
                )
                (t 
                    (setf token-name `(SYNTAX ERROR ,lex-tokenizable CANNOT BE TOKENIZED))
                    (setf general-error t)
                )
            )
        )
    )

    (setf token '())
    (setf token (append token (list lex-tokenizable)))
    (setf token (append token token-name))
    (setf result (append result (list token)))
    (if leading-digit-error
        (exitWithError "Identifiers Cannot Start With Leading Digit(s)")
    )
    (if leading-zero-error
        (exitWithError "Values Cannot Start With Leading Zero(s)")
    )
    (if general-error
        (exitWithError (concatenate 'string lex-tokenizable " Cannot Be Tokenized"))
    )
    (setf is-mult nil)
    (setf is-comment nil)
    (setf comment-candidate nil)
    (setf leading-zero nil)
    (setf leading-digit nil)
    (setf leading-zero-error nil)
    (setf leading-digit-error nil)
    (setf lex-str (string ""))
)

(defun isExpressionEnd(token-list)
    (setf op_counter 0)
    (setf cp_counter 0)
    (loop for token in token-list do
        (if (string= "(" (car token))
            (setf op_counter (+ 1 op_counter))
        )
        (if (string= ")" (car token))
            (setf cp_counter (+ 1 cp_counter))
        )
    )
    (= op_counter cp_counter)
)

(defun exitWithError(&optional err)
    (format t "Expression: ")
    (loop for el in result do
        (format t "~a " (car el))
    )
    (format t "~%Validation: Syntax Error ")
    (if err
        (format t err)
        (format t "Expression Not Recognized")
    )
    (format t "~%")
    (exit)
)

(defun addVar(id val)
    (loop for var in variables do
        (if (string= id (var-id var))
            (return-from addVar (var-val var))
        )
    )
    (setf newVar (make-instance 'var))
    (setf (var-id newVar) id)
    (setf (var-val newVar) val)
    (setf variables (append variables (list newVar)))
    (return-from addVar val)
)

(defun updateVar(id newVal)
    (loop for var in variables do
        (if (string= id (var-id var))
            (return-from updateVar (setf (var-val var) newVal))
        )
    )
    (setf newVar (make-instance 'var))
    (setf (var-id newVar) id)
    (setf (var-val newVar) newVal)
    (setf variables (append variables (list newVar)))
    (return-from updateVar newVal)
)

(defun getVar(id)
    (loop for var in variables do 
        (if (string= id (var-id var))
            (return-from getVar (var-val var))
        )
    )
    (exitWithError (concatenate 'string "Identifier " (string id) " Not Found"))
)

(defun start(token-list)
    (setf exp-result (input token-list))
    (if exp-result
        (progn
            (format t "Expression: ")
            (loop for token in token-list do
                (format t "~a " (car token))
            )
            (format t "~%Validation: Syntax OK~%Result: ")
            (if (equal exp-result 'empty-list)
                (format t "~a~%~%" nil)
                (format t "~a~%~%" exp-result)
            )
        )
        (exitWithError)
    )
    (setf result nil)
)

(defun input(token-list)
    (if (string= (cadr (car token-list)) 'COMMENT)
        (return-from input "Comment")
    )
    (setf input-result (expi token-list))
    (if input-result
        (return-from input input-result)
    )
    (setf input-result (explisti token-list))
    (if input-result
        (return-from input input-result)
        (return-from input nil)
    )
)

(defun expi(token-list)
    (let
        (
            (counter 0)
            (is-plus nil)
            (is-minus nil)
            (is-mult nil)
            (is-div nil)
            (is-set nil)
            (is-if nil)
            (is-defvar nil)
            (first-exp nil)
            (second-exp nil)
            (third-exp nil)
            (id nil)
        )
        (cond
            ((or (string= (cadr (car token-list)) 'IDENTIFIER) (string= (cadr (car token-list)) 'VALUE))
                (setf indexes (append '(0) indexes))
                (if (string= (cadr (car token-list)) 'VALUE)
                    (return-from expi (parse-integer (car (car token-list)))) 
                )

                (if (string= (cadr (car token-list)) 'IDENTIFIER)
                    (return-from expi (getVar (car (car token-list))))
                )
            )
            ((string= (cadr (car token-list)) 'OP_OP)
                (loop for i from 1 to (- (length token-list) 1) do
                    (cond
                        ((or is-plus is-minus is-mult is-div)
                            (cond
                                ((= counter 1)
                                    (setf first-exp (expi (nthcdr i token-list)))
                                    (if (not first-exp)
                                        (return-from expi nil)
                                    )
                                    (setf i (+ i (car indexes)))
                                    (setf indexes (delete (car indexes) indexes))
                                    (setf counter (+ 1 counter))
                                )

                                ((= counter 2)
                                    (setf second-exp (expi (nthcdr i token-list)))
                                    (if (not second-exp)
                                        (return-from expi nil)
                                    )
                                    (setf i (+ i (car indexes)))
                                    (setf indexes (delete (car indexes) indexes))
                                    (setf counter (+ 1 counter))
                                )

                                ((= counter 3)
                                    (cond
                                        ((string= (cadr (nth i token-list)) 'OP_CP)
                                            (setf indexes (append (list i) indexes))
                                            (cond
                                                (is-plus
                                                    (return-from expi (+ first-exp second-exp))
                                                )

                                                (is-minus
                                                    (return-from expi (- first-exp second-exp))
                                                )

                                                (is-mult
                                                    (return-from expi (* first-exp second-exp))
                                                )

                                                (is-div
                                                    (return-from expi (/ first-exp second-exp))
                                                )
                                            )
                                        )

                                        (t
                                            (return-from expi nil)
                                        )
                                    )
                                )
                            )
                        )

                        ((or is-set is-defvar)
                            (cond
                                ((= counter 1)
                                    (if (not (string= (cadr (nth i token-list)) 'IDENTIFIER))
                                        (return-from expi nil)
                                    )
                                    (setf id (car (nth i token-list)))
                                    (setf counter (+ 1 counter))
                                )

                                ((= counter 2)
                                    (setf first-exp (expi (nthcdr i token-list)))
                                    (if (not first-exp)
                                        (return-from expi nil)
                                    )
                                    (setf i (+ i (car indexes)))
                                    (setf indexes (delete (car indexes) indexes))
                                    (setf counter (+ 1 counter))
                                )

                                ((= counter 3)
                                    (cond
                                        ((string= (cadr (nth i token-list)) 'OP_CP)
                                            (setf indexes (append (list i) indexes))
                                            (cond
                                                (is-set
                                                    (return-from expi (updateVar id first-exp))
                                                )

                                                (is-defvar
                                                    (return-from expi (addVar id first-exp))
                                                )
                                            )
                                        )

                                        (t
                                            (return-from expi nil)
                                        )
                                    )
                                )
                            )
                        )

                        (is-if
                            (cond 
                                ((= counter 1)
                                    (setf first-exp (expb (nthcdr i token-list)))
                                    (if (not first-exp)
                                        (return-from expi nil)
                                    )
                                    (setf i (+ i (car indexes)))
                                    (setf indexes (delete (car indexes) indexes))
                                    (setf counter (+ 1 counter))
                                )

                                ((= counter 2)
                                    (setf second-exp (explisti (nthcdr i token-list)))
                                    (if (not second-exp)
                                        (return-from expi nil)
                                    )
                                    (setf i (+ i (car indexes)))
                                    (setf indexes (delete (car indexes) indexes))
                                    (setf counter (+ 1 counter))
                                )

                                ((= counter 3)
                                    (cond
                                        ((string= (cadr (nth i token-list)) 'OP_CP)
                                            (setf indexes (append (list i) indexes))
                                            (if (= first-exp 1)
                                                (setf first-exp t)
                                                (setf first-exp nil)
                                            )
                                            (if first-exp
                                                (return-from expi second-exp)
                                                (return-from expi 'empty-list)
                                            )
                                        )

                                        (t
                                            (setf third-exp (explisti (nthcdr i token-list)))
                                            (if (not third-exp)
                                                (return-from expi nil)
                                            )
                                            (setf i (+ i (car indexes)))
                                            (setf indexes (delete (car indexes) indexes))
                                            (setf counter (+ 1 counter))
                                        )
                                    )
                                )

                                ((= counter 4)
                                    (cond
                                        ((string= (cadr (nth i token-list)) 'OP_CP)
                                            (setf indexes (append (list i) indexes))
                                            (if (= first-exp 1)
                                                (setf first-exp t)
                                                (setf first-exp nil)
                                            )
                                            (if first-exp
                                                (return-from expi second-exp)
                                                (return-from expi third-exp)
                                            )
                                        )

                                        (t
                                            (return-from expi nil)
                                        )
                                    )
                                )
                            )
                        )

                        ((string= (cadr (nth i token-list)) 'OP_PLUS)
                            (setf is-plus t)
                            (setf counter (+ 1 counter))
                        )

                        ((string= (cadr (nth i token-list)) 'OP_MINUS)
                            (setf is-minus t)
                            (setf counter (+ 1 counter))
                        )

                        ((string= (cadr (nth i token-list)) 'OP_MULT)
                            (setf is-mult t)
                            (setf counter (+ 1 counter))
                        )

                        ((string= (cadr (nth i token-list)) 'OP_DIV)
                            (setf is-div t)
                            (setf counter (+ 1 counter))
                        )

                        ((string= (cadr (nth i token-list)) 'KW_SET)
                            (setf is-set t)
                            (setf counter (+ 1 counter))
                        )

                        ((string= (cadr (nth i token-list)) 'KW_DEFVAR)
                            (setf is-defvar t)
                            (setf counter (+ 1 counter))
                        )

                        ((string= (cadr (nth i token-list)) 'KW_IF)
                            (setf is-if t)
                            (setf counter (+ 1 counter))
                        )

                        (t
                            (return-from expi nil)
                        )
                    )
                )
            )

            (t
                (return-from expi nil)
            )
        )
    )
)

(defun expb(token-list)
    (let
        (
            (counter 0)
            (is-and nil)
            (is-or nil)
            (is-not nil)
            (is-equal nil)
            (is-expb nil)
            (is-expi nil)
            (first-exp nil)
            (second-exp nil)
        )
        (cond
            ((or (string= (cadr (car token-list)) 'KW_TRUE) (string= (cadr (car token-list)) 'KW_FALSE))
                (setf indexes (append '(0) indexes))
                (if (string= (cadr (car token-list)) 'KW_TRUE)
                    (return-from expb 1) 
                )

                (if (string= (cadr (car token-list)) 'KW_FALSE)
                    (return-from expb 0)
                )
            )

            ((string= (cadr (car token-list)) 'OP_OP)
                (loop for i from 1 to (- (length token-list) 1) do
                    (cond
                        ((or is-and is-or)
                            (cond
                                ((= counter 1)
                                    (setf first-exp (expb (nthcdr i token-list)))
                                    (if (not first-exp)
                                        (return-from expb nil)
                                    )
                                    (setf i (+ i (car indexes)))
                                    (setf indexes (delete (car indexes) indexes))
                                    (setf counter (+ 1 counter))
                                )

                                ((= counter 2)
                                    (setf second-exp (expb (nthcdr i token-list)))
                                    (if (not second-exp)
                                        (return-from expb nil)
                                    )
                                    (setf i (+ i (car indexes)))
                                    (setf indexes (delete (car indexes) indexes))
                                    (setf counter (+ 1 counter))
                                )

                                ((= counter 3)
                                    (cond
                                        ((string= (cadr (nth i token-list)) 'OP_CP)
                                            (setf indexes (append (list i) indexes))
                                            (if (= first-exp 1)
                                                (setf first-exp t)
                                                (setf first-exp nil)
                                            )
                                            (if (= second-exp 1)
                                                (setf second-exp t)
                                                (setf second-exp nil)
                                            )
                                            (cond
                                                (is-and
                                                    (if (and first-exp second-exp)
                                                        (return-from expb 1)
                                                        (return-from expb 0)
                                                    )
                                                )

                                                (is-or
                                                    (if (or first-exp second-exp)
                                                        (return-from expb 1)
                                                        (return-from expb 0)
                                                    )
                                                )
                                            )
                                        )

                                        (t
                                            (return-from expb nil)
                                        )
                                    )
                                )
                            )
                        )

                        (is-not
                            (cond
                                ((= counter 1)
                                    (setf first-exp (expb (nthcdr i token-list)))
                                    (if (not first-exp)
                                        (return-from expb nil)
                                    )
                                    (setf i (+ i (car indexes)))
                                    (setf indexes (delete (car indexes) indexes))
                                    (setf counter (+ 1 counter))
                                )

                                ((= counter 2)
                                    (cond
                                        ((string= (cadr (nth i token-list)) 'OP_CP)
                                            (setf indexes (append (list i) indexes))
                                            (cond
                                                ((= first-exp 0)
                                                    (return-from expb 1)
                                                )

                                                ((= first-exp 1)
                                                    (return-from expb 0)
                                                )
                                            )
                                        )

                                        (t
                                            (return-from expb nil)
                                        )
                                    )
                                )
                            )
                        )

                        (is-equal
                            (cond
                                ((= counter 1)
                                    (setf first-exp (expb (nthcdr i token-list)))
                                    (if (not first-exp)
                                        (progn
                                            (setf is-expb nil)
                                            (setf first-exp (expi (nthcdr i token-list)))
                                            (if (not first-exp)
                                                (return-from expb nil)
                                                (setf is-expi t)
                                            )
                                        )
                                        (progn
                                            (setf is-expb t)
                                            (setf is-expi nil)
                                        )
                                    )
                                    (setf i (+ i (car indexes)))
                                    (setf indexes (delete (car indexes) indexes))
                                    (setf counter (+ 1 counter))
                                )

                                ((= counter 2)
                                    (if is-expb
                                        (setf second-exp (expb (nthcdr i token-list)))
                                    )
                                    (if is-expi
                                        (setf second-exp (expi (nthcdr i token-list)))
                                    )
                                    (if (not second-exp)
                                        (return-from expb nil)
                                    )
                                    (setf i (+ i (car indexes)))
                                    (setf indexes (delete (car indexes) indexes))
                                    (setf counter (+ 1 counter))
                                )

                                ((= counter 3)
                                    (cond
                                        ((string= (cadr (nth i token-list)) 'OP_CP)
                                            (setf indexes (append (list i) indexes))
                                            (if (equal first-exp second-exp)
                                                (return-from expb 1)
                                                (return-from expb 0)
                                            )
                                        )

                                        (t
                                            (return-from expb nil)
                                        )
                                    )
                                )
                            )
                        )

                        ((string= (cadr (nth i token-list)) 'KW_AND)
                            (setf is-and t)
                            (setf counter (+ 1 counter))
                        )

                        ((string= (cadr (nth i token-list)) 'KW_OR)
                            (setf is-or t)
                            (setf counter (+ 1 counter))
                        )

                        ((string= (cadr (nth i token-list)) 'KW_NOT)
                            (setf is-not t)
                            (setf counter (+ 1 counter))
                        )

                        ((string= (cadr (nth i token-list)) 'KW_EQUAL)
                            (setf is-equal t)
                            (setf counter (+ 1 counter))
                        )

                        (t
                            (return-from expb nil)
                        )
                    )
                )
            )

            (t
                (return-from expb nil)
            )
        )
    )
)

(defun explisti(token-list)
    (let 
        (
            (counter 0)
            (is-concat nil)
            (is-append nil)
            (is-values nil)
            (first-exp nil)
            (second-exp nil)
        )
        (cond
            ((string= (cadr (car token-list)) 'KW_NIL)
                (setf indexes (append '(0) indexes))
                (return-from explisti 'empty-list)
            )

            ((string= (cadr (car token-list)) 'OP_APOSTROPHE)
                (cond
                    ((string= (cadr (cadr token-list)) 'OP_OP)
                        (loop for i from 2 to (- (length token-list) 1) do
                            (cond
                                (is-values
                                    (cond
                                        ((= counter 1)
                                            (setf first-exp (_values (nthcdr i token-list)))
                                            (if (not first-exp)
                                                (return-from explisti nil)
                                            )
                                            (setf i (- (+ i (car indexes)) 1))
                                            (setf indexes (delete (car indexes) indexes))
                                            (setf counter (+ 1 counter))
                                        )

                                        ((= counter 2)
                                            (cond
                                                ((string= (cadr (nth i token-list)) 'OP_CP)
                                                    (setf indexes (append (list i) indexes))
                                                    (return-from explisti first-exp)
                                                )

                                                (t
                                                    (return-from explisti nil)
                                                )
                                            )
                                        )
                                    )
                                )

                                ((string= (cadr (nth i token-list)) 'OP_CP)
                                    (setf indexes (append (list i) indexes))
                                    (return-from explisti 'empty-list)
                                )

                                (t
                                    (setf is-values t)
                                    (setf counter (+ 1 counter))
                                    (setf i (- i 1))
                                )
                            )
                        )
                    )

                    (t
                        (return-from explisti nil)
                    )
                )
            )

            ((string= (cadr (car token-list)) 'OP_OP)
                (loop for i from 1 to (- (length token-list) 1) do
                    (cond
                        ((or is-concat is-append)
                            (cond
                                ((= counter 1)
                                    (if is-concat
                                        (setf first-exp (explisti (nthcdr i token-list)))
                                    )
                                    (if is-append
                                        (setf first-exp (expi (nthcdr i token-list)))
                                    )
                                    (if (not first-exp)
                                        (return-from explisti nil)
                                    )
                                    (setf i (+ i (car indexes)))
                                    (setf indexes (delete (car indexes) indexes))
                                    (setf counter (+ 1 counter))
                                )

                                ((= counter 2)
                                    (setf second-exp (explisti (nthcdr i token-list)))
                                    (if (not second-exp)
                                        (return-from explisti nil)
                                    )
                                    (setf i (+ i (car indexes)))
                                    (setf indexes (delete (car indexes) indexes))
                                    (setf counter (+ 1 counter))
                                )

                                ((= counter 3)
                                    (cond
                                        ((string= (cadr (nth i token-list)) 'OP_CP)
                                            (if (equal first-exp 'empty-list)
                                                (setf first-exp nil)
                                            )
                                            (if (equal second-exp 'empty-list)
                                                (setf second-exp nil)
                                            )
                                            (setf indexes (append (list i) indexes))
                                            (cond
                                                (is-concat
                                                    (return-from explisti (append first-exp second-exp))
                                                )

                                                (is-append
                                                    (return-from explisti (append second-exp (list first-exp)))
                                                )
                                            )
                                        )

                                        (t
                                            (return-from explisti nil)
                                        )
                                    )
                                )
                            )
                        )

                        ((string= (cadr (nth i token-list)) 'KW_CONCAT)
                            (setf is-concat t)
                            (setf counter (+ 1 counter))
                        )

                        ((string= (cadr (nth i token-list)) 'KW_APPEND)
                            (setf is-append t)
                            (setf counter (+ 1 counter))
                        )

                        (t
                            (return-from explisti nil)
                        )
                    )
                )
            )

            (t
                (return-from explisti nil)
            )
        )
    )
)

(defun _values(token-list)
    (let
        (
            (other-values '())
            (value-list '())
            (list-len nil)
        )
        (cond
            ((string= (cadr (car token-list)) 'VALUE)
                (setf value-list (append value-list (list (parse-integer (car (car token-list))))))
                (setf other-values (_values (cdr token-list)))
                (if other-values
                    (setf value-list (append value-list other-values))
                )
                (setf list-len (+ 1 (car indexes)))
                (setf indexes (delete (car indexes) indexes))
                (setf indexes (append (list list-len) indexes))
                (return-from _values value-list)
            )

            (t
                (setf indexes (append '(0) indexes))
                (return-from _values nil)
            )
        )
    )
)

(defun isKeyword(kw-target)
    (setf index 0)
    (loop for kw in keywords do
        (if (equal (string-upcase kw-target) (string kw))
            (return-from isKeyword t)
        )
        (setf index (+ 1 index))
    )
    nil
)

(defun isOperator(op-target)
    (setf index 0)
    (loop for op in operators do
        (if (equal (string op-target) op)
            (return-from isOperator t)
        )
        (setf index (+ 1 index))
    )
    nil
)

(gppinterpreter)