/* SPDX-License-Identifier: GPL-3.0
 * MF-DRV
 * Work in progress driver for the MF Sensor Series
 *
 * COPYRIGHT NOTICE
 * Copyright (C) 2024-2026 0x4248 and contributors
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the license is not changed.
 *
 * This software is free and open source. Licensed under the GNU general
 * public license version 3.0 as published by the Free Software Foundation.
 */

#define D0_PIN 1
#define A0_PIN 0

int a0Value = 0;
int d0Value = 0;

void setup()
{
	pinMode(D0_PIN, INPUT);
	pinMode(A0_PIN, INPUT);
	Serial.begin(9600);
}

void loop()
{
	// Im still figuring out what each of the pins do.
	a0Value = analogRead(A0_PIN);
	d0Value = analogRead(D0_PIN);
	Serial.print("A0: ");
	Serial.print(a0Value);
	Serial.print(" D0: ");
	Serial.println(d0Value);
	delay(100);
}