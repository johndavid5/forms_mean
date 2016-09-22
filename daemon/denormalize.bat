REM ...denormalization tests...
REM .\build\sec_forms_daemon.exe --manual-denormalize-all-forms 2>&1 | tee denormalize-all.out
REM "WESTERN INTERNATIONAL SECURITIES, INC." -- <filer> matches cik example
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "9999999997-16-022382" 2>&1 | tee d.out
REM - "WATERS CORP /DE/" -- <issuer> matches cik example
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001127602-16-035587" 2>&1 | tee d.out
REM "ICAHN ENTERPRISES HOLDINGS L.P." <filed_by> matches cik example...
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001047469-16-009551" 2>&1 | tee d.out
REM "MCGEE SUSAN B" -- <reporting_owner> matches cik example
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0000754811-16-000105" 2>&1 | tee d.out
REM -- Form "4" - "Universal Display Corp /PA/" "0001005284-16-000120"
REM -- forgot to escape the dn_company_name as "Universal Display Corp \/PA\/"
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001005284-16-000120" 2>&1 | tee d.out
REM
REM 9/21/2016:
REM Remaining forms without filer name and filer cik...
REM 0001005284-16-000119	4	2016-01-04		
REM 0001005284-16-000121	4	2016-01-04		
REM 0001005284-16-000122	4	2016-01-04		
REM 0001005284-16-000123	4	2016-01-04		
REM 0001005284-16-000124	4	2016-01-04		
REM 0001445546-16-005950	487	2016-01-04		
REM 0001193805-16-002159	4	2016-01-05		
REM 0000000000-16-060290	UPLOAD	2016-01-05		
REM 0000899243-16-010645	3	2016-01-04		
REM 0000899243-16-010646	4	2016-01-04		
REM 0000899243-16-010696	4	2016-01-04		
REM 0000908662-16-000376	4	2016-01-04		
REM 0000908662-16-000377	4	2016-01-04		
REM 0000921895-16-002927	4	2016-01-04		
REM 0001104659-16-087732	4	2016-01-04		
REM 0001104659-16-087757	4	2016-01-04		
REM 0001104659-16-087779	3	2016-01-04		
REM 0001209191-16-087549	4	2016-01-04		
REM 0001209191-16-088022	4	2016-01-04		
REM 0001209191-16-088023	4	2016-01-04		
REM 0001398344-16-008678	4	2016-01-04		
REM 0001565778-16-000017	4	2016-01-04		
REM 0001638599-16-000466	4	2016-01-04		
REM 0001638599-16-000467	4	2016-01-04		
REM 0000899243-16-010872	4	2016-01-05		
REM 0000899243-16-010955	4	2016-01-05		
REM 0000899243-16-010956	4	2016-01-05		
REM 0000899243-16-010972	4	2016-01-05		
REM 0000899243-16-010975	4	2016-01-05		
REM 0000921895-16-002940	4	2016-01-05		
REM 0001062993-16-006920	4	2016-01-05		
REM 0001104659-16-088209	4	2016-01-05		
REM 0001123292-16-001486	4	2016-01-05		
REM 0001127602-16-035619	4	2016-01-05		
REM 0001127602-16-035870	4	2016-01-05		
REM 0001140361-16-046693	4	2016-01-05		
REM 0001144204-16-074048	4	2016-01-05		
REM 0001144204-16-074193	4	2016-01-05		
REM 0001144204-16-074214	4	2016-01-05		
REM 0001209191-16-089400	4	2016-01-05		
REM 0001398344-16-008704	4	2016-01-05
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001005284-16-000119" 2>1 | tee denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001005284-16-000121" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001005284-16-000122" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001005284-16-000123" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001005284-16-000124" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001445546-16-005950" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001193805-16-002159" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0000000000-16-060290" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0000899243-16-010645" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0000899243-16-010646" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0000899243-16-010696" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0000908662-16-000376" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0000908662-16-000377" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0000921895-16-002927" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001104659-16-087732" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001104659-16-087757" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001104659-16-087779" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001209191-16-087549" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001209191-16-088022" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001209191-16-088023" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001398344-16-008678" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001565778-16-000017" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001638599-16-000466" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001638599-16-000467" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0000899243-16-010872" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0000899243-16-010955" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0000899243-16-010956" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0000899243-16-010972" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0000899243-16-010975" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0000921895-16-002940" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001062993-16-006920" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001104659-16-088209" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001123292-16-001486" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001127602-16-035619" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001127602-16-035870" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001140361-16-046693" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001144204-16-074048" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001144204-16-074193" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001144204-16-074214" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001209191-16-089400" 2>1 | tee -a denormalize.out
REM .\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001398344-16-008704" 2>1 | tee -a denormalize.out
.\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "0001005284-16-000119" 2>1 | tee dn.out
