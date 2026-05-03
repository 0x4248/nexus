! SPDX-License-Identifier: GPL-3.0
! matrix
!
! matrix.f90
!
! COPYRIGHT NOTICE
! Copyright (C) 2024-2026 0x4248 and contributors
! This program is free software: you can redistribute it and/or modify
! it under the terms of the GNU General Public License as published by
! the Free Software Foundation, either version 3 of the License, or
! (at your option) any later version.
!
! Redistribution and use in source and binary forms, with or without
! modification, are permitted provided that the license is not changed.


program matrix
    implicit none

    ! Declare variables
		! C = result
    integer, parameter :: n = 3
    real :: a(n, n), b(n, n), c(n, n)
    integer :: i, j

    ! Initialize matrices A and B
    do i = 1, n
        do j = 1, n
            a(i, j) = real(i + j)
            b(i, j) = real(i - j)
        end do
    end do

    ! Print matrices A and B
    print *, 'Matrix A:'
    do i = 1, n
        print '(3F5.1)', (a(i, j), j = 1, n)
    end do

    print *, ''

    print *, 'Matrix B:'
    do i = 1, n
        print '(3F5.1)', (b(i, j), j = 1, n)
    end do

    print *, ''


    ! Perform matrix operations
    ! Addition
    c = a + b

    print *, 'Matrix C = A + B:'
    do i = 1, n
        print '(3F5.1)', (c(i, j), j = 1, n)
    end do

    print *, ''

    ! Subtraction
    c = a - b

    print *, 'Matrix C = A - B:'
    do i = 1, n
        print '(3F5.1)', (c(i, j), j = 1, n)
    end do

    print *, ''

    ! Multiplication
    c = a * b

    print *, 'Matrix C = A * B:'
    do i = 1, n
        print '(3F5.1)', (c(i, j), j = 1, n)
    end do

    print *, ''

    ! Division
    c = a / b

    print *, 'Matrix C = A / B:'
    do i = 1, n
        print '(3F5.1)', (c(i, j), j = 1, n)
    end do

end program matrix
