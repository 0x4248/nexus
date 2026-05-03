! SPDX-License-Identifier: GPL-3.0
! Factorial
!
! factorial.f90
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

program factorial
    implicit none

    integer :: n, i
    integer :: result

    print *, "Enter a number: "
    read *, n

    result = 1

    do i = 1, n
        result = result * i
    end do

    print *, "Factorial of ", n, " is ", result
end program factorial
