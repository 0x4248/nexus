! SPDX-License-Identifier: GPL-3.0
! adding in fortran
!
! add.f90
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


program add
    implicit none
    integer :: a, b, result
    a = 10
    b = 20
    result = a + b
    print *, 'The result is: ', result
end program add
