start_server {tags {"hellotype"}} {
    test {HELLOTYPE.INSERT key value} {
        r del hellotype1
        assert_equal 1 [r HELLOTYPE.INSERT hellotype1 1]
        assert_equal 2 [r HELLOTYPE.INSERT hellotype1 2]
        assert_equal 3 [r HELLOTYPE.INSERT hellotype1 3]
        assert_equal 4 [r HELLOTYPE.INSERT hellotype1 4]
        assert_equal 5 [r HELLOTYPE.INSERT hellotype1 5]
    }

    test  { HELLOTYPE.LEN key } {
        assert_equal 5 [r HELLOTYPE.LEN hellotype1 ]
    }

    test  {HELLOTYPE.RANGE key start count } {
        assert_equal 1 [r HELLOTYPE.RANGE hellotype1 1 1 ]
        assert_equal {1 2} [r HELLOTYPE.RANGE hellotype1 1 2 ]
        assert_equal {1 2 3 4 5} [r HELLOTYPE.RANGE hellotype1 1 5 ]
    }
}
