function wait(ms){
	return new Promise(resolve => setTimeout(resolve, ms));
}

async function runTests() {
	console.log("Starting tests...");
	console.log("Waiting 5 seconds...")

	await wait(5000);
	console.log("Tests Completed");
}

runTests();
