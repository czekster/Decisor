# decisorGUI
Binaries for the decisor software (using Analytic Hierarchy Process - AHP)
--
## Introduction
This is a short manual for the software, main funcionalities and how to use it, in general.

AHP is a Multicriteria Decision Method (MCDM), also called Multicriteria Decision Analysis (MCDA) by some authors. It is used by many researchers and companies around the globe. It is equipped with a straightforward mechanism to allow complex decisions to be done where modellers input criteria and alternatives and their relationships in a numerical scale. It was invented by [Thomas L. Saaty](https://en.wikipedia.org/wiki/Thomas_L._Saaty) in 1980.

There are several software on the market that handles AHP, for example:

However, those tools lack user friendliness or are proprietary, with a few exceptions. Our tool was conceived to bridge the gap between easiness to use and availability. We offer a simple interface where modellers easily enter criteria and alternatives and assign values among the entities, among other functionalities.

Our software is suited for decision problems with multiple criteria and alternatives. Literature on the subject suggests to have no more than 7 criteria or 7 alternatives at once, however, the software will handle a much larger number. The problem will be the number of needed pairwise comparisons that must be input into the model. It was coded in [Qt](https://www.qt.io/), and it is initially suited for MS-Windows platforms, however, depending on demand, we may consider a GNU/Linux version.

## decisorGUI functionalities
- easy to use interface
- open and save models very intuitively
- multiple judgement scales
- Group Decision Making
- B.O.C.R. (Benefits, Opportunities, Costs, Risks) analysis
- easy export to .csv file readable by MS-Excel or other spreadsheet software

## AHP basic notions
A model is comprised by three 'entities': an *O*bjective, a list of *C*riteria, and a list of *A*lternatives.
![AHP model](https://github.com/unisc/decisorGUI/blob/master/model-ahp.png)

After defining the model, the modeller should assign judgements between criteria and then, per criterion, judgements among alternatives. This process is called pairwise comparison and it involves selection relative weights (from several scales, the simplest one been originally defined by T. L. Saaty as odd numbers from 1 to 9), filling the internal matrices accordingly. What the numerical method does is to

## More information on AHP
Understand the underpinnings of AHP is rather simple, there is a [Wikipedia](https://en.wikipedia.org/wiki/Analytic_hierarchy_process) page with several discussion and information.

## Decision problems
Decision problems usually arises on several domain applications, such as:
- Supplier Selection Problem (SSP): how to choose best suppliers given several competing criteria?
- Management: should I buy a machine or hire another professional? who should I fire based on performance?
- Location: where should I build my next branch?

## Manual
The decisorGUI is a very intuitive and simple tool to model decision problems using AHP.
