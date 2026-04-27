! SPDX-License-Identifier: GPL-3.0
! Prime number
! Calculates if a number is prime or not
!
! prime_number.f90
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

program prime_number
    implicit none
    integer :: n, i
    logical :: is_prime

    print *, 'Enter a number:'
    read *, n

    is_prime = .true.
    do i = 2, n-1
        if (mod(n, i) == 0) then
            is_prime = .false.
            exit
        end if
    end do

    if (is_prime) then
        print *, n, 'is a prime number.'
    else
        print *, n, 'is not a prime number.'
    end if

end program prime_number
