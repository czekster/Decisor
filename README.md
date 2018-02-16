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

## User Manual
The **Decisor** software is a very intuitive and simple tool to model decision problems using AHP.
We will be using a dummy model (very simple) and also the model decribed by in the [Wikipedia-AHP](https://en.wikipedia.org/wiki/Analytic_hierarchy_process) page, where a decision model to hire someone is under consideration.

The software is shipped with a number of models in the /Examples folder. Our working example based on Wikipedia is called **leader.txt**.

### Start screen
![Start Screen](https://github.com/unisc/decisorGUI/blob/master/images/start-screen.png)

At this point it is possible to:
- Insert an **Objective**
- Insert up to two **Criteria**
- Insert up to two **Alternatives**

If needed more criteria or alternatives, the modeller should press the button **ADD** where the Decisor software will add another criterion or alternative to the model.
If some criterion or alternative needs to be excluded, the modeller should choose one (by clicking at its checkbox) and then pressing the **Delete Selected** button.

It is a good practice to always press the **Update** button after a criterion or alternative is edited. The system will internally updates the screens to reflect the changes (the tab bars).

### Creating a simple model
After entering some text into the input fields, and pressing the **Update** button, the system will automatically create three more tabs on top, one for the criterion-criterion pairwise comparisons, one for the comparisons among alternatives for the first criterion and another one for the second criterion, as shown by next figure (we are using a simplistic *dummy model* of two criteria and two alternatives, just as a simple example):
![Adding criteria and alternatives](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model.png)

It is possible to see the tabs that were created on top. If the user selects the first tab (**Criteria**):
![Criteria selected](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-criteria.png)

To conduct pairwise comparisons for the *Effectiveness* criterion:
![Effectiveness selected](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-effectiveness.png)

To conduct pairwise comparisons for the *Punctuality* criterion:
![Punctuality selected](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-punctuality.png)

If one decides to update the model, i.e., by adding more alternatives, one should go back to the **Model** tab, press **ADD** and then insert *Richard* as a new alternave (for instance):
![New alternative Richard](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-richard.png)

After we added the alternative we have pressed the **Update** button as well. See that the tab **Criteria** is unchanged, however, for the criterion (both), it has added a new Alternative (in this case, *Richard*):
![New alternative Richard](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-richard2.png)

### Pairwise comparisons
Now, we proceed to the pairwise comparisons. In the **Criteria** tab, the modeller should perform judgements among criteria, using a scale. For the sake of our example, we will use the standard Fundamental Scale proposed by Saaty, where elements are entered in a 1 to 9 scale, where 1 is indifferent, and 9 is of high importance. Next table explains the scale in detail:

|*Scale* | *Definition* | *Explanation*|
|:---:|:-------|:------------|
|**1**|Same importance, indifference. |Both elements contribute equally towards the objective.|
|**3**|Small importance one over another. |One element is slightly favored with respect to (W.R.T.) the other.|
|**5**|Considerable importance, essential. |One characteristic is highly important WRT the other.|
|**7**|High importance. |One aspect is strongly more important WRT the other in comparison, dominating the other.|
|**9**|Highest importance, absolute. |Highest importance between two elements, with high level of confidence of its relevance.|
|**2,4,6,8**|Intermediate values among the scale. |A good compromise between two characteristics acting as a reasonable balance.|
|**Reciprocals** |If a compared pair at position [i,j] has value over zero, then the pair at position [j,i] should have assigned 1/value, i.e., a reciprocal value. |The reciprocals values cause the model to have certain equilibrium WRT the elements.|

We have decided to enter importance by assigning different weights among criteria such as efficiency is considerably more important (5) than punctuality:
![Pairwise judgements](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-criteria-pairwise.png)

The modeller has entered with 5 in the first line, where Effectiveness (line parameter) is of high importance w.r.t. Punctuality. Note that the software has already computed an intermediary weight vector, despite the fact that the pairwise comparisons for the alternatives are not yet entered. This could be **swithed off** if desired by clicking in *Settings, Automatic Calculation*.

Next, we should conduct the judgements for each criterion in respect to the possible alternatives:
**Effectiveness with respect to all alternatives**
![Pairwise judgements for effectiveness](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-effectiveness-pairwise.png)

**Punctuality with respect to all alternatives**
![Pairwise judgements for punctuality](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-punctuality-pairwise.png)

Note that the software presents the choice for this problem: management should hire Richard, despite the fact that Nancy is more punctual than others. This is due to the fact that the company prefers effectiveness over punctuality, as stated in the criteria tab for the model.

## Working with more sophisticated models
In next figure, we have clicked in *Open*, then selected the *Examples* folder and then we have choosed the **leader.txt** model):
![Leader Model](https://github.com/unisc/decisorGUI/blob/master/images/leader-model.png)



