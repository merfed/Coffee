package koward;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;

import jm2lib.blizzard.io.BlizzardInputStream;
import jm2lib.blizzard.io.BlizzardOutputStream;
import jm2lib.blizzard.wow.M2;
import jm2lib.blizzard.wow.classic.Model;

/**
 * CollisionPorter Main class.
 * 
 * @author Koward
 *
 */
public class CollisionPorter {
	private static final String HELP = "(Frog gets the collision of BigChair) \njava -jar CollisionPorter.jar -in Frog.m2 -src BigChair.m2 -out FrogNew.m2\n";
	private static final Options options;
	static {
		options = new Options();
		options.addOption("in", "input", true, "path to original model file");
		options.addOption("src", "source", true, "path to model with the collisions you want");
		options.addOption("out", "output", true, "path to output file");
	}

	/**
	 * Main method.
	 * 
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args) throws Exception {
		System.out.println("[[ CollisionPorter by Koward v1.0.0 ]]");
		HelpFormatter formatter = new HelpFormatter();
		CommandLineParser parser = new DefaultParser();
		CommandLine cmd = parser.parse(options, args);
		if (!cmd.hasOption("input") || !cmd.hasOption("source") || !cmd.hasOption("output")) {
			System.err.println("Error : Some arguments have not been specified.");
			formatter.printHelp(HELP, options);
			System.exit(1);
		}

		BlizzardInputStream in = new BlizzardInputStream(cmd.getOptionValue("input"));
		M2 inputM2 = (M2) in.readObject();
		in.close();
		BlizzardInputStream src = new BlizzardInputStream(cmd.getOptionValue("source"));
		M2 srcM2 = (M2) src.readObject();
		src.close();
		Model inModel = (Model) inputM2.getModel();
		Model srcModel = (Model) srcM2.getModel();

		// Copy collisions
		inModel.boundingBox = srcModel.boundingBox;
		inModel.boundingSphereRadius = srcModel.boundingSphereRadius;
		inModel.collisionBox = srcModel.collisionBox;
		inModel.collisionSphereRadius = srcModel.collisionSphereRadius;
		inModel.boundingTriangles = srcModel.boundingTriangles;
		inModel.boundingVertices = srcModel.boundingVertices;
		inModel.boundingNormals = srcModel.boundingNormals;

		BlizzardOutputStream out = new BlizzardOutputStream(cmd.getOptionValue("output"));
		out.writeObject(inputM2);
		System.out.println(cmd.getOptionValue("output") + " written.");
		out.close();
	}
}
