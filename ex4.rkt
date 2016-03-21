#lang racket

(require openssl/sha1)	

(define hex "0e3647e8592d35514a081243582536ed3de6734059001e3f535ce6271032")

(define (read-chunks input-port)
  (let loop
    ([accu '()])
    (define nxt (read-line input-port 'any))
    (if (eof-object? nxt)
        accu
        (loop (cons nxt accu)))))

(for ([possible-char (range 255)])
     (println (bytes->string/utf-8 (apply bytes (for/list ([byte (bytes-length (hex-string->bytes hex))])
                                                          (bitwise-xor possible-char byte))))))