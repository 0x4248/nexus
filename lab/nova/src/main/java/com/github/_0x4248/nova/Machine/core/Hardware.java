package com.github._0x4248.nova.Machine.core;

public interface Hardware {

    String id();

    default void onAttach(Machine machine) {
    }

    default void onDetach(Machine machine) {
    }
}
