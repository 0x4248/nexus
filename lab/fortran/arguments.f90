! SPDX-License-Identifier: GPL-3.0
! Arguments
!
! arguments.f90
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

program arguments
    implicit none

    integer :: i
    character(len=100) :: arg

    do i = 1, command_argument_count()
        call get_command_argument(i, arg)
        write(*,*) 'Argument ', i, ': ', trim(adjustl(arg))
    end do
end program
