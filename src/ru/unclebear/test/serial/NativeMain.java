package ru.unclebear.test.serial;

import java.io.File;
import java.util.Arrays;

public class NativeMain {

	public static native String[] getSerials();
	
	static {
		System.load(System.getProperty("user.dir") + File.separator
				 + "target/libserial.so");
	}
	
	public static void main(String[] args) {
		System.out.println("------------------------");
		System.out.println(Arrays.toString(getSerials()));
		System.out.println("------------------------");
	}

}
