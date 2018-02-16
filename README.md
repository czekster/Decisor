# Decisor
Download the Decisor software (using Analytic Hierarchy Process - AHP)
--
## Introduction
This is a short manual for the software called **Decisor**, main funcionalities and how to use it, in general.

AHP is a *Multicriteria Decision Method* (MCDM), also called *Multicriteria Decision Analysis* (MCDA) by some authors. It is used by many researchers and companies around the globe. It is equipped with a straightforward mechanism to allow complex decisions to be done where modellers input criteria and alternatives and their relationships in a numerical scale. It was invented by [Thomas L. Saaty](https://en.wikipedia.org/wiki/Thomas_L._Saaty) in 1980.

There are several software on the market that handles AHP, for example:
- SuperDecisions (proprietary)
- Expert Choice (proprietary)
- Transparent Choice (proprietary)
- PriEsT (free, hard to use, too many features)
- Some online AHP solutions such as *AHP Online Calculator*, easyAHP (free for 3 alternatives and 3 criteria)

However, those tools either lack user friendliness and support, or are proprietary, with a few exceptions. Our tool was conceived to bridge the gap between easiness to use and availability. We offer a simple interface where modellers easily enter criteria and alternatives and assign values among the entities, among other functionalities.

Our software is suited for decision problems with multiple criteria and alternatives. Literature on the subject suggests to have no more than 7 criteria or 7 alternatives at once, however, the software is prepared to handle a much larger number. The problem will be the number of needed pairwise comparisons that must be input into the model. It was coded in [Qt](https://www.qt.io/), and it is initially suited for MS-Windows platforms, however, depending on demand, we may consider a GNU/Linux version.

## Decisor functionalities
- Easy to use interface
- Open and save models very intuitively
- Edit criteria and alternatives in a quick manner, i.e., adding, renaming, excluding
- Multiple judgement scales (i.e., fundamental, exponential, etc.)
- Group Decision Making (i.e., input several models, from several stakeholders and then the software applies a geometric mean)
- B.O.C.R. (Benefits, Opportunities, Costs, Risks) analysis
- Easy export to .csv (*comma separated file*) file readable by MS-Excel or other spreadsheet software
- Human readable model format input (it is possible to directly edit the model in a Text Editor and then open in the tool)

## AHP basic notions
A model is comprised by three 'entities': an **O**bjective, a list of **C**riteria, and a list of **A**lternatives.
![AHP model](https://github.com/unisc/decisorGUI/blob/master/images/model-ahp.png)

After defining the model, the modeller should assign judgements between criteria and then, per criterion, judgements among alternatives. This process is called pairwise comparison and it involves selection relative weights (from several scales, the simplest one been originally defined by T. L. Saaty as odd numbers from 1 to 9 - even numbers could be used as well as intermediary values), filling the internal matrices accordingly. 

What the numerical method does is to normalize internal matrices and compute weights according to the judgment scales provided by the users. Then, it takes into account the relation between each criterion and the alternatives and present the modeller with a ranking of alternatives with numerical outputs for decision makers to decide the best alternative.

## More information on AHP
Understand the underpinnings of AHP is rather simple, there is a [Wikipedia](https://en.wikipedia.org/wiki/Analytic_hierarchy_process) page with several discussion and information.

## Decision problems
Decision problems usually arises on several domain applications, such as:
- Supplier Selection Problem (SSP): how to choose best suppliers given several competing criteria?
- Management: should I buy a machine or hire another professional? who should I fire based on performance?
- Location: where should I build my next branch?
- Financial: how to invest?

And several other problems. The strength of AHP is its ability to easily create a model and extract actionable inferences, aiding enourmously decision making.

## User manual
Please, take a look at the [User Manual](https://github.com/unisc/decisorGUI/blob/master/user-manual) for further information on how to use the software.

