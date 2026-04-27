! SPDX-License-Identifier: GPL-3.0
! fortran-filetest
!
! file.f90
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

program main
	implicit none ! Explicitly declare all variables

	! Write hello world to a file
	print *, 'Writing file...'

	! open (unit, file, status)
    open(1, file='test.txt', status='unknown')

	! write (unit, format) list
    write(1, *) 'Hello, world!'

	! close (unit)
    close(1)

	print *, 'File written!'

end program main
