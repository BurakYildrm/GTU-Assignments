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


(defconstant keywords '(AND OR NOT EQUAL LESS NIL LIST APPEND CONCAT SET DEFFUN FOR IF EXIT LOAD DISP TRUE FALSE))
(defconstant keyword-names '(KW_AND KW_OR KW_NOT KW_EQUAL KW_LESS KW_NIL KW_LIST KW_APPEND KW_CONCAT KW_SET KW_DEFFUN KW_FOR KW_IF KW_EXIT KW_LOAD KW_DISP KW_TRUE KW_FALSE))
(defconstant operators '("+" "-" "/" "*" "(" ")" "**" "," "\""))
(defconstant operator-names '(OP_PLUS OP_MINUS OP_DIV OP_MULT OP_OP OP_CP OP_DBLMULT OP_COMMA OP_OC OP_CC))
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
        (setf input-file (open "input.txt"))
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
                                (tokenize lex-str)
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
                                    (tokenize c)
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
                                    (tokenize c)
                                )
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
                            ;(setf leading-zero nil)
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
    (close input-file)
)

(defun tokenize(lex-tokenizable)
    (cond
        (is-comment
            (setf token-name '(COMMENT))
        )

        (leading-digit-error
            (setf token-name '(SYNTAX ERROR IDENTIFIERS CANNOT START WITH LEADING DIGITS))
        )

        (leading-zero-error
            (setf token-name '(SYNTAX ERROR INTEGERS CANNOT START WITH LEADING ZEROS))
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
                        ;(setf token-name (list (nth (position (string lex-tokenizable) operators) operator-names)))
                    )
                )
                (t 
                    (setf token-name `(SYNTAX ERROR ,lex-tokenizable CANNOT BE TOKENIZED))
                )
            )
        )
    )

    (setf token '())
    (setf token (append token (list lex-tokenizable)))
    (setf token (append token token-name))
    (setf result (append result (list token)))
    (setf is-mult nil)
    (setf is-comment nil)
    (setf comment-candidate nil)
    (setf leading-zero nil)
    (setf leading-digit nil)
    (setf leading-zero-error nil)
    (setf leading-digit-error nil)
    (setf lex-str (string ""))
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

(gppinterpreter 'input.g++)

(defun printList(le)
    (format t "~a~%" le)
)

(mapcar #'printList result)
